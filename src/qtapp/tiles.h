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
#ifndef _TILES_H
#define _TILES_H

#include <vector>
#include <sl/axis_aligned_box.hpp>
#include <sl/fixed_size_point.hpp>
#include <qimage.h>
#include <qobject.h>
#include <cassert>

class QRect;

class tiles
{ 
  
 public:

  tiles();
  virtual ~tiles();

  void setImagePtr(QImage* img);
  void destroyImage();
  QImage generateTileMask(const int k);

  inline int width() const {
    int result =0;
    if ( m_image ) result = m_image->width(); 
    return result;
  }
  
  inline int height() const {
    int result =0;
    if ( m_image ) result = m_image->height(); 
    return result;
  }
  
  inline bool goodX( const int x ){
    return (x >= 0) && (x < width());
  }

  inline bool goodY( const int y ){
    return (y >= 0) && (y < height());
  }
  

  inline int tileDim() const {
    return m_tile_dim;
  }
  
  inline void setTileDim(const int value) {
    assert(value >= 0);
    m_tile_dim = value;
  }
  
  inline bool goodImage() const {
    bool result =false;
    if (!(m_image==NULL)) result = !m_image->isNull();
    return result;
  }
  
  inline int sizeX() const {
    assert(m_tile_dim > 0);
    int result = 0;    
    if (goodImage()) {
      float f = (float) width() / (float) tileDim();
      result = (int) ceil(f);
    }
    return result;
  }

  inline int sizeY() const {
    assert(m_tile_dim > 0);
    int result = 0;
    if (goodImage()) {
      float f = (float) height() / (float) tileDim();
      result = (int) ceil(f);
    }
    return result;
  }


 
  inline int size() const {
    return sizeX() * sizeY();
  }

  
  inline bool goodTileIndex(const int i) const {
    return (i >= 0 && i < size());
  }

  inline void setfillcolor(sl::color3f c){
    m_fill_color = c;
  }

  inline sl::color3f getfillcolor(){
    return m_fill_color;
  }

  inline int luminance(QRgb rgb){
    return (int(0.299*qRed(rgb)+0.587*qGreen(rgb)+0.114*qBlue(rgb)));
  }

  inline int crominance_b(QRgb rgb){
    return (int(-0.169*qRed(rgb)-0.331*qGreen(rgb)+0.500*qBlue(rgb)));
  }

  inline int crominance_r(QRgb rgb){
    return (int(0.500*qRed(rgb)-0.419*qGreen(rgb)-0.081*qBlue(rgb)));
  }

  inline QRgb setAlpha(QRgb rgb,int alpha){
    QRgb temp = qRgba(qRed(rgb),qGreen(rgb),qBlue(rgb),alpha);
    return temp; 
  }

  QRect rectTile(const int i);
  sl::aabox2f  weightTile(const int i);
  QImage tile(const int i);

  inline void setTh(int t) {
    m_th = t;
  }
  

 protected:
  
  QImage* m_image;
  int m_tile_dim;
  std::vector<sl::point3f> vertex;
  sl::color3f m_fill_color;
  int m_th;
};

#endif // _TILES_H
