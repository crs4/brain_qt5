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
#ifndef _BSTEXIMAGE_H
#define _BSTEXIMAGE_H

#include "bsViewer3D.h"
#include <qimage.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <vector>
#include "tiles.h"

class qtCommandHistory;
class bsGLPainter;

class bsTexImage : public bsViewer3D
{
  Q_OBJECT

public:

  bsTexImage( QWidget* parent = 0);

  virtual ~bsTexImage();

public slots:

void generateTextureTiles();
void recalculateBoundingBox();
 void handlingMousePressEvent ( QMouseEvent * e );
 void handlingMouseMoveEvent ( QMouseEvent * e );

signals:
void imageChanged();


public:

 void setCurrentImage(QImage* image);
 void resetP();
 float cameraFar() const;
 float cameraNear() const;
 float maxCameraDistance() const;
 void translation(sl::tuple2i delta_t);
 void destroyTextures();
 bool invariant();
 void render();
 void loadTrueImage( const QString fileName );
 void calculateImageBox(float correction);


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

   inline sl::column_vector3f tv() {
    sl::point3f point_f;
    sl::point3f point_init;

    point_f = cameraV()*sl::point3f(0.0f,0.0f,0.0f);
   
    return (point_f-point_init);
  } 

   inline void setfillcolor(sl::color3f c){
     m_current_image.setfillcolor(c) ;
   }

inline sl::color3f getfillcolor(){
     return m_current_image.getfillcolor();
   }

  // GL Functions
  virtual void initializeGL();
  virtual void paintGL();
  
 protected:
  std::vector<GLuint> m_texture_tiles;
  tiles m_current_image;
  sl::tuple2i m_pixel_init;
  int m_status;
  
};


#endif // _BSTEXIMAGE_H


