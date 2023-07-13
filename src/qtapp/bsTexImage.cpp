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
#include "bsTexImage.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QMouseEvent>
#include "bsGLPainter.h"
#include <iostream>
#include <sl/affine_map.hpp>
#include <GL/glu.h>

const int M_IDLE=0;
const int M_PRESSED_LEFT=1;
const int M_PRESSED_RIGHT=2;

// bsTexImage 
bsTexImage::bsTexImage(QWidget* parent)
    : bsViewer3D(parent){

  connect(this, SIGNAL(imageChanged()), this, SLOT(recalculateBoundingBox()));
  connect(this, SIGNAL(imageChanged()), this, SLOT( generateTextureTiles()));
  connect(this, SIGNAL(mousePress(QMouseEvent *)), this, SLOT( handlingMousePressEvent(QMouseEvent *) ));
  connect(this, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT( handlingMouseMoveEvent(QMouseEvent *) ));
  m_current_image.setTileDim(512);
  m_pixel_init  = 0 , 0;
  m_status=M_IDLE;
  reset();
}

bsTexImage::~bsTexImage() {
  makeCurrent();
  destroyTextures();
}

// GL Functions
void bsTexImage::initializeGL() {
#ifndef NDEBUG
  std::cerr << "bsTexImage InitializeGL" << std::endl;
#endif 
}

void bsTexImage::paintGL() {
#ifndef NDEBUG
  //  std::cerr << "bsTexImage PaintGL" << std::endl;
#endif

  if( invariant() ) {
    glClearColor( 0.0, 0.0, 0.0, 0.0 ); 
    glEnable( GL_CULL_FACE );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);    
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( cameraAP().to_pointer() );    
    glMatrixMode( GL_MODELVIEW );
    glLoadMatrixf( cameraV().to_pointer() );
    glEnable( GL_TEXTURE_2D ); 
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE); 
    
    glPushMatrix();
    {   
      for (size_t i=0; i<m_texture_tiles.size(); i++) {	
	glBindTexture(GL_TEXTURE_2D,m_texture_tiles[i]);	
	glBegin(GL_QUADS); 
	{
	  sl::aabox2f w = m_current_image.weightTile(i);
	  float x0 = m_init_box[0][0] + (m_init_box[1][0]-m_init_box[0][0]) * w[0][0];
	  float x1 = m_init_box[0][0] + (m_init_box[1][0]-m_init_box[0][0]) * w[1][0];
	  float y0 = m_init_box[1][1] + (m_init_box[0][1]-m_init_box[1][1]) * w[0][1];
	  float y1 = m_init_box[1][1] + (m_init_box[0][1]-m_init_box[1][1]) * w[1][1];	  
	  glTexCoord2f(0.0f,0.0f);
	  glVertex3f(x0, y1, m_init_box[1][2]);
	  glTexCoord2f(1.0f,0.0f);
	  glVertex3f(x1, y1, m_init_box[1][2]);
	  glTexCoord2f(1.0f,1.0f);
	  glVertex3f(x1, y0, m_init_box[1][2]);
	  glTexCoord2f(0.0f,1.0f);
	  glVertex3f(x0, y0, m_init_box[1][2]);
	}
	glEnd();
      }
    }
    glPopMatrix();
  }    
}


void bsTexImage::setCurrentImage(QImage* image) {
  if ( !(image==NULL) && !(image->isNull()) ) {
    m_current_image.setImagePtr(image);
    setCameraA(1.8f);    
    emit imageChanged();
    updateGL();
  }
  else { std::cerr << "IMAGE NOT LOADED" << std::endl;}
}

void bsTexImage::recalculateBoundingBox(){
  float image_aspect_ratio = m_current_image.height() == 0 ? 1.0f : (float(m_current_image.width())) / ((float)(m_current_image.height()));
  sl::aabox3f box = sl::aabox3f(sl::point3f(-image_aspect_ratio, -1.0f, -1.0f), sl::point3f(image_aspect_ratio,1.0f , 1.0f));
  setInitBox(box);
}

void bsTexImage::resetP() {
  float dim = m_init_box.diagonal().two_norm();
  m_current_cameraP=sl::linear_map_factory3f::ortho(-dim, dim,-dim, dim,cameraNear(),cameraFar());
}

float bsTexImage::maxCameraDistance() const{
  return 1.5f*m_init_box.diagonal().two_norm();
}


float bsTexImage::cameraFar() const{
  return maxCameraDistance() + 0.5f*m_init_box.diagonal().two_norm();
}


float bsTexImage::cameraNear() const{
  return maxCameraDistance() - 0.5f*m_init_box.diagonal().two_norm();
}

void bsTexImage::translation(sl::tuple2i delta_t){

  float alfa= -tv()[2]*1.5f;
  float delta_tx = ((float)delta_t[0]*alfa)/(float)width();
  float delta_ty = - ((float)delta_t[1]*alfa)/(float)height();
  sl::column_vector3f delta_t_world;
  delta_t_world = delta_tx, delta_ty, 0.0f;
  sl::affine_map3f delta_T = sl::linear_map_factory3f::translation(delta_t_world);
  sl::affine_map3f temp_V = delta_T * cameraV();  
  setCameraV(temp_V);
}

void bsTexImage::handlingMousePressEvent ( QMouseEvent * e ) {
  m_pixel_init[0]=e->x();
  m_pixel_init[1]=e->y();
  if (e->button()==Qt::LeftButton){
    m_status=M_PRESSED_LEFT;
  }
  if (e->button()==Qt::RightButton){
    m_status=M_PRESSED_RIGHT;
  } 
}

void bsTexImage::handlingMouseMoveEvent ( QMouseEvent * e ) { 
  sl::tuple2i point_f;
  point_f= e->x(), e->y();
  sl::tuple2i delta_pixel;
  delta_pixel[0]  = point_f[0] - m_pixel_init[0];
  delta_pixel[1]  = point_f[1] - m_pixel_init[1];
  if (m_status == M_PRESSED_LEFT){
    translation(delta_pixel);
  } 
  updateGL();
  m_pixel_init=point_f;
} 

void bsTexImage::generateTextureTiles(){
  destroyTextures();
  glEnable( GL_TEXTURE_2D );
  for (int i=0; i<m_current_image.size(); i++) {
    appendTexture();
    glGenTextures(1,&m_texture_tiles[i]);
    glBindTexture(GL_TEXTURE_2D,m_texture_tiles[i]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);    
    glTexImage2D(GL_TEXTURE_2D,0,3,m_current_image.tileDim(),m_current_image.tileDim(),0,GL_RGBA,GL_UNSIGNED_BYTE,m_current_image.tile(i).bits());
  }
} 


void bsTexImage::destroyTextures() {
  for (size_t i=0; i<m_texture_tiles.size(); i++) {
    glDeleteTextures(1, &m_texture_tiles[i]);
  }
  m_texture_tiles.clear();
}

bool bsTexImage::invariant() {
  return (int(m_texture_tiles.size())==m_current_image.size());
}

void bsTexImage::render() {  
  glEnable( GL_TEXTURE_2D );
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glPushMatrix();
  {   
    for (size_t i=0; i<m_texture_tiles.size(); i++) {	
      glBindTexture(GL_TEXTURE_2D,m_texture_tiles[i]);
      glBegin(GL_QUADS); 
      {
	sl::aabox2f w = m_current_image.weightTile(i);
	float x0 = m_init_box[0][0] + (m_init_box[1][0]-m_init_box[0][0]) * w[0][0];
	float x1 = m_init_box[0][0] + (m_init_box[1][0]-m_init_box[0][0]) * w[1][0];
	float y0 = m_init_box[1][1] + (m_init_box[0][1]-m_init_box[1][1]) * w[0][1];
	float y1 = m_init_box[1][1] + (m_init_box[0][1]-m_init_box[1][1]) * w[1][1];	  
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(x0, y1, m_init_box[1][2]);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(x1, y1, m_init_box[1][2]);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(x1, y0, m_init_box[1][2]);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(x0, y0, m_init_box[1][2]);
      }
      glEnd();
    }
  }
  glPopMatrix();
  glDisable( GL_TEXTURE_2D );
}

  void bsTexImage::loadTrueImage( const QString fileName ) {
    QImage* img = new QImage(fileName);
    if ( !(img==NULL) && !(img->isNull()) ) {
      m_current_image.setImagePtr(img);
    }
  }

void bsTexImage::calculateImageBox(float correction){
  float image_aspect_ratio = m_current_image.height() == 0 ? 1.0f : (float(m_current_image.width())) / ((float)(m_current_image.height()));
  float value = (image_aspect_ratio * correction );
  sl::aabox3f box = sl::aabox3f(sl::point3f(-value, -correction, -1.0f), sl::point3f( value,correction , 1.0f));
  setInitBox(box);
} 

