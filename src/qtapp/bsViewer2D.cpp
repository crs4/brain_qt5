//+++HDR+++
//======================================================================
//   This file is part of the BRAIN software framework.
//   Copyright (C) 2009-2022 by CRS4, Pula, Italy.
//
//   For more information, visit the CRS4 Visual Computing Group
//   web pages at http://www.crs4.it/vic/
//
//   This file may be used under the terms of the GNU General Public
//   License as published by the Free Software Foundation and appearing
//   in the file LICENSE included in the packaging of this file.
//
//   CRS4 reserves all rights not expressly granted herein.
//  
//   This file is provided AS IS with NO WARRANTY OF ANY KIND, 
//   INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS 
//   FOR A PARTICULAR PURPOSE.
//
//======================================================================
//---HDR---//
#include "bsViewer2D.h"
#include "qtCommandHistory.h"
#include "bsCmdPanZoom.h"
#include "bsGLPainter.h"
#include <iostream>

// Private functions

float bsViewer2D::normalizedTranslation(int i, int step) const{
  float t = 0.5f + 0.5f *(step/100.0f);
  float d = 0.5f*m_init_box.diagonal()[i];
  return (1-t)*(1./zoomFactor(i)-d)+t*(-1./zoomFactor(i)+d);
}


float bsViewer2D::normalizedZoomFactor(int factor) const{
  return factor/100.0f;
}


// bsViewer2D 

bsViewer2D::bsViewer2D( QWidget* parent )
    : bsViewer(parent) {
  m_init_box=sl::aabox3f(sl::point3f(-1.0f, -1.0f, -1.0f), sl::point3f(1.0f, 1.0f, 1.0f));
  m_current_zoom_factor = 1.0f;
  m_current_translation=sl::column_vector2f(0.0f, 0.0f);
  m_current_aspect_ratio = 1.0f;
  m_grid_flag = false;
  setCellStepX(10000.0f);
  setCellStepY(10000.0f);
}


bsViewer2D::~bsViewer2D() {
}


sl::affine_map3f bsViewer2D::cameraV() const { 
  float tz = -m_init_box[1][2];
  sl::affine_map3f result = 
    sl::linear_map_factory3f::translation(sl::column_vector3f(-m_init_box.center()[0]-m_current_translation[0],
							      -m_init_box.center()[1]-m_current_translation[1],
							      tz));
  return result;
}


sl::projective_map3f bsViewer2D::cameraP() const {
  sl::column_vector3f scale;
  sl::column_vector3f translate;

  float d = std::max(m_init_box[1][0] - m_init_box[0][0],
			   m_init_box[1][1] - m_init_box[0][1]);
  d = std::max(d, m_init_box[1][2] - m_init_box[0][2]);

  for (int i=0; i<3; i++) {
    float s = (i==2) ? -zoomFactor(i) : zoomFactor(i);
    scale[i] = (d == 0.0f) ? s : s * 2.0f / d;
    translate[i] = (i==2) ? -1.0f : 0.0f;
  }

  sl::projective_map3f result=
      sl::linear_map_factory3f::translation(translate) *
      sl::linear_map_factory3f::scaling(scale);

  return result;
}


float bsViewer2D::zoomFactor(int i) const {
  assert(i>=0 && i<= 2);

  float result = -10.0;
  switch (i) {
  case 0: result = m_current_zoom_factor; break;
  case 1: result = m_current_zoom_factor * m_current_aspect_ratio; break;
  case 2: result = 1.0f; break;
  default:
    assert(false);
  }
  return result;
}


float bsViewer2D::worldToPixel() const {
  return (zoomFactor(0) * width()) / (2.0f * m_init_box.diagonal()[0]);
}


void bsViewer2D::initializeGL() {
#ifndef NDEBUG
  std::cerr << "initializeGL" << std::endl;
#endif
}

void bsViewer2D::setCellStepX(float value) {
  m_cell_stepX = value;
  updateGL();
}

void bsViewer2D::setCellStepY(float value) {
  m_cell_stepY = value;
  updateGL();
}

void bsViewer2D::setGrid(bool b) {
  m_grid_flag = b;
  updateGL();
}

void bsViewer2D::drawGrid() {
  if (m_grid_flag) {
    float dim = 2*m_init_box.diagonal().two_norm();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    glColor3f(0.6f, 0.6f, 0.6f);
    glLineWidth(1);
    
    int nx = (int)(dim / m_cell_stepX);
    float cx = m_init_box.center()[0];
    for (int i=-nx; i<nx; i++) {
      float x = cx+m_cell_stepX*i;
      glBegin(GL_LINE_LOOP); {
	glVertex2f(x,dim);
	glVertex2f(x,-dim);
      } glEnd();
    }

    int ny = (int)(dim / m_cell_stepY);
    float cy = m_init_box.center()[1];
    for (int i=-ny; i<ny; i++) {
      float y = cy+m_cell_stepY*i;
      glBegin(GL_LINE_LOOP); {
	glVertex2f(dim,y);
	glVertex2f(-dim,y);
      } glEnd();
    }
  }
}

void bsViewer2D::paintGL() {
#ifndef NDEBUG
  std::cerr << "paintGL" << std::endl;
#endif
  qglClearColor( Qt::black ); 
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glShadeModel(GL_FLAT);
  glDisable(GL_LIGHTING);
  glColor3f(1.0, 0.0, 0.0);
  glDisable(GL_COLOR_MATERIAL);

  glMatrixMode( GL_PROJECTION );
  glLoadMatrixf( cameraP().to_pointer() );

  glMatrixMode( GL_MODELVIEW );
  glLoadMatrixf( cameraV().to_pointer() );

  glPushMatrix(); {
    glColor3f(1.,0.,0.);
    glBegin(GL_POLYGON); {
      glVertex2f(m_init_box[0][0],m_init_box[0][1]);
      glVertex2f(m_init_box[1][0],m_init_box[0][1]);
      glVertex2f(m_init_box[1][0],m_init_box[1][1]);
      glVertex2f(m_init_box[0][0],m_init_box[1][1]);
    } glEnd();
    glColor3f(0.,0.,1.);
  } glPopMatrix();

  glPushMatrix(); {
    glScalef(0.5,0.5,1.0);
    glBegin(GL_POLYGON); {
      glVertex2f(m_init_box[0][0],m_init_box[0][1]);
      glVertex2f(m_init_box[1][0],m_init_box[0][1]);
      glVertex2f(m_init_box[1][0],m_init_box[1][1]);
      glVertex2f(m_init_box[0][0],m_init_box[1][1]);
    } glEnd();
  } glPopMatrix();

 glPushMatrix(); {
     glLineWidth(2);
    glColor3f(1.,1.,0.);
    glBegin(GL_LINES); {
      glVertex2f(m_init_box[0][0],m_init_box[0][1]);
      glVertex2f(m_init_box[1][0],m_init_box[1][1]);
      glVertex2f(m_init_box[0][0],m_init_box[1][1]);
      glVertex2f(m_init_box[1][0],m_init_box[0][1]);
    } glEnd();
    glLineWidth(1);
  } glPopMatrix();

 paintPainters();
}


void bsViewer2D::resizeGL( int w, int h ) {
#ifndef NDEBUG
   std::cerr << "resizeGL" << std::endl;
#endif
   glViewport( 0, 0, (GLint)w, (GLint)h );
   m_current_aspect_ratio = h == 0 ? 1.0f : (float(w)) / ((float)(h));
   updateGL();
}


int bsViewer2D::getZoomFactor() const{
  return (int)(m_current_zoom_factor * 100.0f + 0.5f);
}


int bsViewer2D::getXTranslation() const{
  float d = 0.5*m_init_box.diagonal()[0];
  float t = (m_current_translation[0]-1./m_current_zoom_factor+d)/
      (-2./m_current_zoom_factor+2.*d);
  float val = (t-0.5f)/0.5f * 100.0f;
  int step = (int)((val > 0) ? (val+0.5f) : (val-0.5f));
  return step;
}


int bsViewer2D::getYTranslation() const{
  float d = 0.5*m_init_box.diagonal()[1];
  float t = (m_current_translation[1]-1./m_current_zoom_factor+d)/(-2./m_current_zoom_factor+2.*d);
  float val = (t-0.5f)/0.5f * 100.0f;
  int step = (int)((val > 0) ? (val+0.5f) : (val-0.5f));
  return -step;
}


void bsViewer2D::setCommandProcessor(qtCommandHistory* cmd_processor) {
    m_cmd_processor = (cmd_processor) ? (cmd_processor) : new qtCommandHistory(-1);
}


qtCommandHistory*  bsViewer2D::commandProcessor(){
    return m_cmd_processor;
}


float bsViewer2D::getNormalizedZoomFactor() const{
  return m_current_zoom_factor;
}


float bsViewer2D::getNormalizedXTranslation() const{
  return m_current_translation[0];
}


float bsViewer2D::getNormalizedYTranslation() const{
  return m_current_translation[1];
}


// Public slots

void bsViewer2D::setZoomFactor(int factor) {
    float zf=normalizedZoomFactor(factor);
    if (zf != m_current_zoom_factor){
	bsCmdPanZoom * cmd = new bsCmdPanZoom(this, 'z', zf);
	m_cmd_processor->doit(cmd);
    }
}


void bsViewer2D::setXTranslation(int factor) {
  float tx=normalizedTranslation(0,factor);
  if (tx != m_current_translation[0]){
      bsCmdPanZoom * cmd = new bsCmdPanZoom(this, 'x', tx);
      m_cmd_processor->doit(cmd);
  }
}


void bsViewer2D::setYTranslation(int factor) {
  float ty=normalizedTranslation(1,-factor);
  if (ty != m_current_translation[1]){
      bsCmdPanZoom * cmd = new bsCmdPanZoom(this, 'y', ty);
      m_cmd_processor->doit(cmd);
  }
}


void bsViewer2D::reset() {
  bsCmdPanZoom * cmd = new bsCmdPanZoom(this, 'r', -1);
  m_cmd_processor->doit(cmd);
}


void bsViewer2D::undo() {
  if (m_cmd_processor->doneCount() > 0) {
    m_cmd_processor->undo();
  }
}


void bsViewer2D::redo() {
  if (m_cmd_processor->undoneCount() > 0) {
    m_cmd_processor->redo();
  }
}


// Command

void bsViewer2D::setXTranslationCmd(float tx) {
    m_current_translation[0]=tx;
    emit xTranslationChanged(getXTranslation());
    updateGL();
}


void bsViewer2D::setYTranslationCmd(float ty) {
    m_current_translation[1]=ty;
    emit yTranslationChanged(getYTranslation());
    updateGL();
}


void bsViewer2D::setZoomFactorCmd(float zf) {
    m_current_zoom_factor=zf;
    emit zoomFactorChanged(getZoomFactor());
    updateGL();
}


void bsViewer2D::resetCmd() {
  m_current_zoom_factor = 1.0;
  m_current_translation.to_zero();
  emit xTranslationChanged(0);
  emit yTranslationChanged(0);
  emit zoomFactorChanged(100);
  updateGL();
}
 



