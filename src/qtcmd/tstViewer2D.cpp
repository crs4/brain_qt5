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
#include "tstViewer2D.h"
#include <iostream>

tstViewer2D::tstViewer2D( QWidget* parent, const char* name, const QGLWidget* shareWidget )
    : QGLWidget( parent, name, shareWidget ) {

  m_init_box=sl::aabox3f(sl::point3f(-1, -1,-1),sl::point3f(1, 1,1));
  m_current_zoom_factor = 1.0;
  m_current_translation=sl::column_vector2f(0.0, 0.0);
}


tstViewer2D::~tstViewer2D() {
  makeCurrent();
}



sl::affine_map3f tstViewer2D::cameraV() const { 
  sl::affine_map3f result;
  if( m_current_zoom_factor <= 1.0 ) {
    result = sl::linear_map_factory3f::translation(sl::column_vector3f(-m_init_box.center()[0],
					       -m_init_box.center()[1],
					       0.0));
  } else {
    result = sl::linear_map_factory3f::translation(sl::column_vector3f(-m_init_box.center()[0]-m_current_translation[0],
					       -m_init_box.center()[1]-m_current_translation[1],
					       0.0));
  }

  std::cerr << "V = " << result << std::endl;
 
  return result;
}


sl::projective_map3f tstViewer2D::cameraP() const {
  sl::projective_map3f result;
  result = sl::linear_map_factory3f::scaling(sl::column_vector3f(m_current_zoom_factor,
								m_current_zoom_factor,
								1.0f));
  std::cerr << "P = " << result << std::endl;
 
  return result;
}


void tstViewer2D::setXTranslation( int step ) {
  float t = 0.5 + 0.5 *(step/100.0);
  float d = 0.5*m_init_box.diagonal()[0];
  m_current_translation[0]=(1-t)*(1./m_current_zoom_factor-d)+t*(-1./m_current_zoom_factor+d);
  emit xTranslationChanged(step);
  updateGL();
}

int tstViewer2D::getXTranslation() const {
  float d = 0.5*m_init_box.diagonal()[0];
  float t = (m_current_translation[0]-1./m_current_zoom_factor+d)/(-2./m_current_zoom_factor+2.*d);
  int step = (t-0.5)/0.5 * 100.0;
  return step;
}

void tstViewer2D::setYTranslation( int step ) {
  
  float t = 0.5 + 0.5 *(step/100.0);
  float d = 0.5*m_init_box.diagonal()[1];
  m_current_translation[1]=(1-t)*(1./m_current_zoom_factor-d)+t*(-1./m_current_zoom_factor+d);
  emit yTranslationChanged(step);
  updateGL();
}

int tstViewer2D::getYTranslation() const {
  float d = 0.5*m_init_box.diagonal()[1];
  float t = (m_current_translation[1]-1./m_current_zoom_factor+d)/(-2./m_current_zoom_factor+2.*d);
  int step = (t-0.5)/0.5 * 100.0;
  return step;
}

int tstViewer2D::getZoomFactor() const {
  return m_current_zoom_factor * 100.0;
}

void tstViewer2D::setZoomFactor(int factor) {
  m_current_zoom_factor=factor/100.0;
  if ( m_current_zoom_factor<=1.0) {
    m_current_translation.to_zero();
    emit xTranslationChanged(0);
    emit yTranslationChanged(0);
  }
  emit zoomFactorChanged(factor);
  updateGL();
}

void tstViewer2D::reset() {
  m_current_zoom_factor = 1.0;
  m_current_translation.to_zero();
  emit xTranslationChanged(0);
  emit yTranslationChanged(0);
  emit zoomFactorChanged(100);
  updateGL();
}
 

///////////////////////////////////////////////////////////////////////////////


void tstViewer2D::initializeGL() {
  std::cerr << "initializeGL" << std::endl;
}


void tstViewer2D::paintGL() {
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

}


void tstViewer2D::resizeGL( int w, int h ) {
   std::cerr << "resizeGL" << std::endl;
   glViewport( 0, 0, (GLint)w, (GLint)h );
}




