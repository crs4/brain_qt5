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
#ifndef _BSTEXVIEWER_H
#define _BSTEXVIEWER_H

#include "bsViewer3D.h"
#include <qimage.h>
#include <vector>
#include "tiles.h"

class qtCommandHistory;
class bsGLPainter;

class bsTexViewer : public bsViewer3D
{
  Q_OBJECT

public:

  bsTexViewer( QWidget* parent = 0);

  virtual ~bsTexViewer();

public:
 void setCurrentImage(QImage* image);
 void generateTextureTiles();
 void destroyTextures();
 void render();
 void multirender();
 void loadTrueImage( const QString fileName );
 void loadBufferImage( const QString fileName );
 void calculateImageBox(float correction);
 void generateMaskTiles();
 void generateMultiTiles();

 inline bool goodImage() const {
   return m_current_image.goodImage();
 }

 inline int textureCount() const {
   return m_texture_tiles.size();
 }
 
 inline bool goodTextureIndex(int i) const {
   return i>=0 && i < textureCount();
 }

 inline GLuint m_texture(int i)
   {
     assert(goodTextureIndex(i));
     return m_texture_tiles[i];
   }

 inline void appendTexture() {
   GLuint t;
   m_texture_tiles.push_back(t);
 }

 inline void setfillcolor(sl::color3f c){
   m_current_image.setfillcolor(c) ;
 }

 inline sl::color3f getfillcolor(){
   return m_current_image.getfillcolor();
 }

 inline void setMaskEnable(bool b){
   m_on_mask = b;
 }

 inline void setTh(int th){
   m_current_image.setTh(th);
 }

  
 protected:
 std::vector<GLuint> m_texture_tiles;
 tiles m_current_image;
 bool m_on_mask;
};

#endif // _BSTEXVIEWER_H

