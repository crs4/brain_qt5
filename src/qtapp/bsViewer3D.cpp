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
#include "bsViewer3D.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QWheelEvent>
#include "bsGLPainter.h"
#include <iostream>
#include <sl/affine_map.hpp>

// bsViewer3D 
bsViewer3D::bsViewer3D(QWidget* parent)
  : bsViewer(parent) {

  m_current_zoom=1.0f;
  m_current_aspect_ratio=1.0f;

  m_init_box=sl::aabox3f(sl::point3f(-1.0f, -1.0f, -1.0f), sl::point3f(1.0f, 1.0f, 1.0f));
  connect(this, SIGNAL(mouseWheelChanged(QWheelEvent *)), this, SLOT( handlingMouseWheelChangedEvent(QWheelEvent *) )); 
  resetA();
  resetP();
  resetV();
}

bsViewer3D::~bsViewer3D() {
  //  makeCurrent();
}

// Geometries

float bsViewer3D::maxCameraDistance() const{
  return 3.0f*m_init_box.diagonal().two_norm();
}


float bsViewer3D::cameraFar() const{
  return maxCameraDistance() + 0.5f*m_init_box.diagonal().two_norm();
}


float bsViewer3D::cameraNear() const{
  return maxCameraDistance() - 0.5f*m_init_box.diagonal().two_norm();
}

float bsViewer3D::worldToPixel() const {
  sl::point3f p;
  p[0]=m_init_box.diagonal()[0];
  p[1]=m_init_box.diagonal()[1];
  p[2]=m_init_box.diagonal()[2];
  return (worldToNormalized(p)[0]*width()/2.0f);

  
}


// A P V Matrix  
sl::affine_map3f bsViewer3D::cameraV() const { 
  return m_current_cameraV;
}

sl::projective_map3f bsViewer3D::cameraP() const {
  return m_current_cameraP;
}

sl::affine_map3f bsViewer3D::cameraA() const { 
  return m_current_cameraA;
}

sl::projective_map3f bsViewer3D::cameraAP() const {
  sl::projective_map3f result=  m_current_cameraA * cameraP();
  return result;
}

// set matrices
void  bsViewer3D::setCameraP(const sl::projective_map3f& P){
  m_current_cameraP = P;
  emit matrixPChanged();
}

void  bsViewer3D::setCameraV(const sl::affine_map3f& V){
  m_current_cameraV = V;
  emit matrixVChanged();
}

void bsViewer3D::setCameraA(const float zoom) {
  assert(zoom!=0.0f);
  m_current_zoom=zoom;
  m_current_cameraA = sl::linear_map_factory3f::scaling(m_current_zoom, m_current_zoom*aspectRatio(), 1.0f);
}

// reset matrices

void bsViewer3D::resetP() {
  float dim = m_init_box.diagonal().two_norm();
  m_current_cameraP=sl::linear_map_factory3f::ortho(-dim, dim,
							-dim, dim,
							cameraNear(),
							cameraFar());
}

void bsViewer3D::resetV() {
  m_current_cameraV = sl::linear_map_factory3f::lookat(m_init_box.center()+
						       sl::column_vector3f(0.0f,0.0f,maxCameraDistance()),
						       m_init_box.center(),
						       0.0f);
}

void bsViewer3D::resetA() {
  m_current_zoom=1.0f;
  m_current_cameraA = sl::linear_map_factory3f::scaling(sl::column_vector3f(1.0f,1.0f*aspectRatio(),1.0f));
}


// GL Functions

void bsViewer3D::initializeGL() {
#ifndef NDEBUG
  std::cerr << "initializeGL" << std::endl;
#endif
}

void bsViewer3D::paintGL() {
#ifndef NDEBUG
  std::cerr << "paintGL" << std::endl;
#endif
  glClearColor(240.0f/255.0f, 240.0f/255.0f, 240.0f/255.0f, 1.0f ); 
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glShadeModel(GL_FLAT);
  glDisable(GL_LIGHTING);
  glColor3f(1.0, 0.0, 0.0);
  glDisable(GL_COLOR_MATERIAL);

  glMatrixMode( GL_PROJECTION );
  glLoadMatrixf( cameraAP().to_pointer() );

  glMatrixMode( GL_MODELVIEW );
  glLoadMatrixf( cameraV().to_pointer() );

  glPushMatrix(); {
    glColor3f(1.,0.,0.);
    glBegin(GL_POLYGON); {
      glVertex3f(m_init_box[0][0],m_init_box[0][1],m_init_box[0][2]);
      glVertex3f(m_init_box[1][0],m_init_box[0][1],m_init_box[0][2]);
      glVertex3f(m_init_box[1][0],m_init_box[0][1],m_init_box[1][2]);
      glVertex3f(m_init_box[0][0],m_init_box[0][1],m_init_box[1][2]);
    } glEnd();
  } glPopMatrix();

  glPushMatrix(); {
    glColor3f(0.,1.,0.);
    glBegin(GL_POLYGON); {
      glVertex3f(m_init_box[0][0],m_init_box[0][1],m_init_box[0][2]);
      glVertex3f(m_init_box[1][0],m_init_box[0][1],m_init_box[0][2]);
      glVertex3f(m_init_box[1][0],m_init_box[1][1],m_init_box[0][2]);
      glVertex3f(m_init_box[0][0],m_init_box[1][1],m_init_box[0][2]);
    } glEnd();
  } glPopMatrix();

 paintPainters();
}


void bsViewer3D::resizeGL( int w, int h ) {
#ifndef NDEBUG
  //std::cerr << "resizeGL" << std::endl;
#endif
   glViewport( 0, 0, (GLint)w, (GLint)h );
   m_current_aspect_ratio = h == 0 ? 1.0f : (float(w)) / ((float)(h));
   setCameraA(m_current_zoom);
   updateGL();
}


// Command processor
void bsViewer3D::setCommandProcessor(qtCommandHistory* cmd_processor) {
    m_cmd_processor = (cmd_processor) ? (cmd_processor) : new qtCommandHistory(-1);
}

qtCommandHistory*  bsViewer3D::commandProcessor(){
    return m_cmd_processor;
}



// reset slots

void bsViewer3D::reset() {
  resetA();
  resetP();
  resetV();
  updateGL();
}

void bsViewer3D::reset_right() {
  resetP();
  m_current_cameraV = sl::linear_map_factory3f::lookat(m_init_box.center()+
						       sl::column_vector3f(maxCameraDistance(),0.0f,0.0f),
						       m_init_box.center(),
						       0.0f);
  updateGL();
}


void bsViewer3D::reset_left() {
  resetP();
  m_current_cameraV = sl::linear_map_factory3f::lookat(m_init_box.center()+
						       sl::column_vector3f(-maxCameraDistance(),0.0f,0.0f),
						       m_init_box.center(),
						       0.0f);
  updateGL();
}

void bsViewer3D::reset_top() {
  resetP();
  m_current_cameraV = sl::linear_map_factory3f::lookat(m_init_box.center()+
						       sl::column_vector3f(0.0f,maxCameraDistance(),0.0f),
						       m_init_box.center(),
						       0.0f);
  updateGL();
}

void bsViewer3D::reset_bottom() {
  resetP();
  m_current_cameraV = sl::linear_map_factory3f::lookat(m_init_box.center()+
						       sl::column_vector3f(0.0f,-maxCameraDistance(),0.0f),
						       m_init_box.center(),
						       0.0f);
  updateGL();
}

void bsViewer3D::reset_front() {
  resetP();
  m_current_cameraV = sl::linear_map_factory3f::lookat(m_init_box.center()+
						       sl::column_vector3f(0.0f,0.0f,maxCameraDistance()),
						       m_init_box.center(),
						       0.0f);
  updateGL();
}

void bsViewer3D::reset_rear() {
  resetP();
  m_current_cameraV = sl::linear_map_factory3f::lookat(m_init_box.center()+
						       sl::column_vector3f(0.0f,0.0f,-maxCameraDistance()),
						       m_init_box.center(),
						       0.0f);
  updateGL();
}


// handling events

void bsViewer3D::handlingMouseWheelChangedEvent ( QWheelEvent * e ) { 
  int versus = e->angleDelta().y();
  float var_zoom = 1.0f;
  if (versus > 0) {
    var_zoom = 0.90f;
  }
  else {
    var_zoom = 1.10f;    
  }
  float zoom = scaleFactor()*var_zoom;
  setCameraA(zoom);
  updateGL();
} 


