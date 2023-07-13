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
#include "bsGLPainter.h"
#include <iostream>


// bsViewer3D 

bsViewer3D::bsViewer3D( QWidget* parent, const char* name, const QGLWidget* shareWidget )
    : QGLWidget( parent, name, shareWidget ) {

  m_init_box=sl::aabox3f(sl::point3f(-1.0f, -1.0f, -1.0f), sl::point3f(1.0f, 1.0f, 1.0f));
  m_current_aspect_ratio = 1.0f;

  float tz = -m_init_box[1][2];
  m_current_cameraV = sl::linear_map_factory3f::translation(sl::column_vector3f(-m_init_box.center()[0],
										-m_init_box.center()[1],tz));

  sl::column_vector3f translate; translate = 0.0f, 0.0f, -1.0f;
  m_current_cameraP =
    sl::linear_map_factory3f::translation(translate);
}


bsViewer3D::~bsViewer3D() {
  makeCurrent();
}

sl::affine_map3f bsViewer3D::cameraV() const { 
  return m_current_cameraV;
}

sl::projective_map3f bsViewer3D::cameraP() const {
  return m_current_cameraP;
}

sl::projective_map3f bsViewer3D::cameraPV() const {
  sl::projective_map3f result= cameraP() * cameraV();
  return result;
}

sl::projective_map3f bsViewer3D::cameraAP() const {
  sl::projective_map3f result=  m_current_cameraA * cameraP();
  return result;
}


///////////////////////////////////////////////////////////////////////////////

void  bsViewer3D::mousePressEvent( QMouseEvent *e ) {
    emit mousePress(e);
}

void  bsViewer3D::mouseReleaseEvent( QMouseEvent *e ) {
    emit mouseRelease(e);
}

void  bsViewer3D::mouseMoveEvent( QMouseEvent *e ) {
    emit mouseMove(e);
}

void  bsViewer3D::mouseDoubleClickEvent( QMouseEvent *e ) {
    emit mouseDoubleClick(e);
}

void bsViewer3D::initializeGL() {
  std::cerr << "initializeGL" << std::endl;
}


void bsViewer3D::paintGL() {
  std::cerr << "paintGL" << std::endl;
  qglClearColor( black ); 
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glDisable(GL_DEPTH_TEST);
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

void bsViewer3D::resizeGL( int w, int h ) {
   std::cerr << "resizeGL" << std::endl;
   glViewport( 0, 0, (GLint)w, (GLint)h );
   float aspect_ratio = h == 0 ? 1.0f : (float(w)) / ((float)(h));
   m_current_cameraA = sl::linear_map_factory3f::scaling(sl::column_vector3f(1.0f,aspect_ratio,1.0f));
}


// Protected 

void bsViewer3D::paintPainters() {
    for (int i=0; i<m_painters.size(); i++) {
	m_painters[i]->paintGL();
    }
}


// Public slots

void  bsViewer3D::setCameraP(const sl::projective_map3f& P){
  m_current_cameraP = P;
  emit matrixPChanged();
}

void  bsViewer3D::setCameraV(const sl::affine_map3f& V){
  m_current_cameraV = V;
  emit matrixVChanged();
}


// Painting

void bsViewer3D::registerPainter(bsGLPainter* p) {
    assert(p);
    m_painters.push_back(p);
}

void bsViewer3D::unregisterPainter(bsGLPainter* p) {
    assert(p);
    int found_index = -1;
    for (int i=0; i<m_painters.size() && found_index < 0; i++) {
	if (m_painters[i] == p) {
	    found_index = i;
	}
    }
    if (found_index >= 0) {
	for (int i= found_index; i<m_painters.size()-1; i++) {
	    m_painters[i] = m_painters[i+1];
	}
	m_painters.pop_back();
    }
}

