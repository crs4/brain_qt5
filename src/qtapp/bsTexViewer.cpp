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
#include "bsTexViewer.h"
#include "qtCommandHistory.h"
#include "bsGLPainter.h"
#include <iostream>
#include <sl/affine_map.hpp>
#include <GL/glu.h>

// bsTexViewer 
bsTexViewer::bsTexViewer(QWidget* parent)
  : bsViewer3D(parent)
{
  m_current_image.setTileDim(256);
}

bsTexViewer::~bsTexViewer() {
  makeCurrent();
  destroyTextures();
}

void bsTexViewer::setCurrentImage(QImage* image) {
  if ( !(image==NULL) && !(image->isNull()) ) {
    m_current_image.setImagePtr(image);
  }
  else { std::cerr << "IMAGE NOT LOADED" << std::endl;}
}

void bsTexViewer::generateTextureTiles(){
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


void bsTexViewer::destroyTextures() {
  for (size_t i=0; i<m_texture_tiles.size(); i++) {
    glDeleteTextures(1, &m_texture_tiles[i]);
  }
  m_texture_tiles.clear();
}


void bsTexViewer::render() {  
  glEnable( GL_TEXTURE_2D );
  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glColor4f(1.0f,1.0f,1.0f,1.0f);
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
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

  void bsTexViewer::loadTrueImage( const QString fileName ) {
    destroyTextures();
    QImage* img = new QImage(fileName);
    if ( !(img==NULL) && !(img->isNull()) ) {
      m_current_image.setImagePtr(img);
      if (m_on_mask){
	generateMaskTiles();
      }
      else {
	generateTextureTiles();
      }
    }
  }

void bsTexViewer::calculateImageBox(float correction){
  float image_aspect_ratio = m_current_image.height() == 0 ? 1.0f : (float(m_current_image.width())) / ((float)(m_current_image.height()));
  float value = (image_aspect_ratio * correction );
  sl::aabox3f box = sl::aabox3f(sl::point3f(-value, -correction, -1.0f), sl::point3f( value,correction , 1.0f));
  setInitBox(box);
} 

void bsTexViewer::generateMaskTiles(){
  destroyTextures();
  glEnable( GL_TEXTURE_2D );
  for (int i=0; i<m_current_image.size(); i++) {
    appendTexture();
    glGenTextures(1,&m_texture_tiles[i]);
    glBindTexture(GL_TEXTURE_2D,m_texture_tiles[i]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);    
    glTexImage2D(GL_TEXTURE_2D,0,4,m_current_image.tileDim(),m_current_image.tileDim(),0,GL_RGBA,GL_UNSIGNED_BYTE,m_current_image.generateTileMask(i).bits());
  }
} 

void bsTexViewer::loadBufferImage( const QString fileName ) {
  
  QImage* img = new QImage(fileName);
  if ( !(img==NULL) && !(img->isNull()) ) {
    m_current_image.setImagePtr(img);   
    generateMultiTiles();    
  }
}

void bsTexViewer::generateMultiTiles(){
  destroyTextures();
  glEnable( GL_TEXTURE_2D );
  for (int i=0; i<m_current_image.size(); i++) {
    appendTexture();
    glGenTextures(1,&m_texture_tiles[i]);
    glBindTexture(GL_TEXTURE_2D,m_texture_tiles[i]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);    
    glTexImage2D(GL_TEXTURE_2D,0,4,m_current_image.tileDim(),m_current_image.tileDim(),0,GL_RGBA,GL_UNSIGNED_BYTE,m_current_image.generateTileMask(i).bits());
  }
} 

void bsTexViewer::multirender() {  
  glEnable( GL_TEXTURE_2D );
  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glColor4f(1.0f,1.0f,1.0f,1.0f);
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
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}
