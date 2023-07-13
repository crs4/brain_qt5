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
#include <tiles.h>
#include <qrect.h>
#include <qgl.h>

tiles::tiles(){
  m_image = NULL;
  //setTileDim(256);
}

tiles::~tiles(){
  //  destroyImage();
}

QRect tiles::rectTile(const int i) {
  assert( goodTileIndex(i) );
  int indY = i/sizeX();
  int indX = i%sizeX();
  int x0 = indX*tileDim();
  int y0 = indY*tileDim();
  int x1 =  x0 + tileDim() - 1;
  int y1 =  y0 + tileDim() - 1;

  QPoint p0 = QPoint(x0,y0);
  QPoint p1 = QPoint(x1,y1);
  QRect result = QRect(p0, p1);
  return result;
}


QImage tiles::tile(const int i) {
  assert(goodTileIndex(i));
  QRect r = rectTile(i);
  QImage t =  m_image->copy(r);
   
  QColor c = QColor((int)(m_fill_color[0]*255.0f),(int)(m_fill_color[1]*255.0f),(int)(m_fill_color[2]*255.0f));
 
  if( r.right()-width()+1 > 0 ) {
    std::cerr << "FILL col tile " << i << std::endl;
    for (int col=tileDim()-(r.right()-width()+1); col < tileDim(); col++) {
       for(int y=0; y < tileDim(); y++) {
	t.setPixel(col,y,c.rgb());
      }
    } 
  }
  if( r.bottom()-height()+1 > 0 ) {
     std::cerr << "FILL row tile " << i << std::endl;
     for (int row=tileDim()-(r.bottom()-height()+1); row < tileDim(); row++) {
       for(int x=0; x < tileDim(); x++) {
	 t.setPixel(x,row,c.rgb());
       }
     } 
  }

  QImage temp = QGLWidget::convertToGLFormat(t);
  std::cerr << "  tile done!  "<< std::endl;
  return temp;
  
}


sl::aabox2f  tiles::weightTile(const int i) {
  assert(goodTileIndex(i));
  sl::point2f p0, p1;
  QRect r = rectTile(i);
  p0 = (float)(r.left())/(float)(width()), (float)(r.top())/(float)(height());
  p1 = (float)(r.right()+1)/(float)(width()), (float)(r.bottom()+1)/(float)(height());
  return sl::aabox2f(p0, p1);
}


void tiles::setImagePtr(QImage* img) {
  destroyImage();
  m_image=img;
  std::cerr << "Image: " << width() << "X" << height() << "  Tile dim= " << tileDim() << "  Size = " << size() << " " << sizeX() << "X" << sizeY() << std::endl;
}


void tiles::destroyImage() {
  if (m_image) {
    delete m_image;
    m_image = NULL;
  }
}

QImage tiles::generateTileMask(const int i){
  assert(goodTileIndex(i));
  QRect r = rectTile(i);
  QImage t =  m_image->copy(r);
 
  //t.setAlphaBuffer(true);
 
  QColor c = QColor((int)(m_fill_color[0]*255.0f),(int)(m_fill_color[1]*255.0f),(int)(m_fill_color[2]*255.0f));  
    
    for (int x=0; x < tileDim(); x++) {
       for(int y=0; y < tileDim(); y++) {

	 if( luminance(t.pixel(x,y)) > m_th ) {
	   QRgb a = setAlpha(t.pixel(x,y),0);	   
	   t.setPixel(x,y,a);	  
	   //t.setPixel(x,y,c.rgb());
	   }
       }
    } 

    if( r.right()-width()+1 > 0 ) {
      std::cerr << "FILL col tile " << i << std::endl;
      for (int col=tileDim()-(r.right()-width()+1); col < tileDim(); col++) {
	for(int y=0; y < tileDim(); y++) {
	  t.setPixel(col,y,c.rgb());
	}
      } 
    }
    if( r.bottom()-height()+1 > 0 ) {
      std::cerr << "FILL row tile " << i << std::endl;
      for (int row=tileDim()-(r.bottom()-height()+1); row < tileDim(); row++) {
	for(int x=0; x < tileDim(); x++) {
	  t.setPixel(x,row,c.rgb());
	}
      } 
    }

    QImage temp = QGLWidget::convertToGLFormat(t);
    //std::cerr << qAlpha(temp.pixel(100,100)) << std::endl;
    //std::cerr << "  tile done!  "<< std::endl;
   
    return temp;

}
