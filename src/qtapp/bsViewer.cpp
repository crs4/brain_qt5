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
#include "bsViewer.h"
#include "bsGLPainter.h"
#include <iostream>
#include <GL/gl.h>

#include <QMouseEvent>
#include <QImage>
//Added by qt3to4:
#include <QWheelEvent>
#include <QEvent>
#include <QKeyEvent>
//#include <QPixmap>


// bsViewer 

bsViewer::bsViewer( QWidget* parent )
    : QGLWidget(parent) {
  m_bkg_color = 240.0f/255.0f, 240.0f/255.0f, 240.0f/255.0f;
  m_fancy_rendering_flag = true;
}


bsViewer::~bsViewer() {
  //  makeCurrent();
}


sl::projective_map3f bsViewer::cameraPV() const {
  sl::projective_map3f result= cameraP() * cameraV();
  return result;
}


///////////////////////////////////////////////////////////////////////////////

void  bsViewer::mousePressEvent( QMouseEvent *e ) {
    emit mousePress(e);
}


void  bsViewer::mouseReleaseEvent( QMouseEvent *e ) {
    emit mouseRelease(e);
}


void  bsViewer::mouseMoveEvent( QMouseEvent *e ) {
    emit mouseMove(e);
}

void  bsViewer::mouseDoubleClickEvent( QMouseEvent *e ) {
    emit mouseDoubleClick(e);
}

void  bsViewer::enterEvent( QEvent *e ) {
    emit mouseEnter(e);
}

void bsViewer::keyPressEvent(  QKeyEvent *e ){
  emit keyPress(e);
}

void bsViewer::keyReleaseEvent(  QKeyEvent *e ){
  emit keyRelease(e);
}

void bsViewer::wheelEvent ( QWheelEvent * e ) {
  emit mouseWheelChanged(e);
}


// Protected 

void bsViewer::paintPainters() {
    for (int i=0; i<(int)m_painters.size(); i++) {
	m_painters[i]->paintGL();
    }
}


// Painting

void bsViewer::registerPainter(bsGLPainter* p) {
    assert(p);
    m_painters.push_back(p);
}


void bsViewer::unregisterPainter(bsGLPainter* p) {
    assert(p);
    int found_index = -1;
    for (int i=0; i<(int)m_painters.size() && found_index < 0; i++) {
	if (m_painters[i] == p) {
	    found_index = i;
	}
    }
    if (found_index >= 0) {
	for (int i= found_index; i<(int)m_painters.size()-1; i++) {
	    m_painters[i] = m_painters[i+1];
	}
	m_painters.pop_back();
    }
}

// Utility

sl::point2f bsViewer::pixelsToNormalized(QMouseEvent *e) const {
    return sl::point2f(2.0f*e->x()/width()-1.0f, -(2.0f*e->y()/height()-1.0f));
} 


sl::point2f bsViewer::worldToNormalized(const sl::point3f& p) const {
    sl::point3f ch = cameraPV() * p;
    return sl::point2f(ch[0], ch[1]);
}


sl::point3f bsViewer::normalizedToWorldNear(const sl::point2f& p) const {
    return cameraPV().inverse() * sl::point3f(p[0],p[1],-1.0f);
}


sl::point3f bsViewer::normalizedToWorldFar(const sl::point2f& p) const {
    return cameraPV().inverse() * sl::point3f(p[0],p[1], 1.0f);
}


sl::point3f bsViewer::worldToCamera(const sl::point3f& p) const {
  return cameraV()*p;
}





QImage bsViewer::getGLPixmap() {
  int w=width();
  int h=height();
  sl::color3f old_col = bkgColor();
  sl::color3f new_col; new_col = 1.0f, 1.0f, 1.0f;
  setBkgColor(new_col);
  updateGL();
  glFlush();
  unsigned char* GLpixmap = new unsigned char [w*h*3];
  glReadBuffer(GL_FRONT);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glReadPixels(0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,GLpixmap);
  QImage pm(w,h,QImage::Format_RGB32);
  for( int r=0; r < h; r++) {
    for( int c=0; c < w; c++) {
      pm.setPixel(c,(h-1-r),qRgba((int)GLpixmap[(r*w+c)*3+0],
			    (int)GLpixmap[(r*w+c)*3+1],
			    (int)GLpixmap[(r*w+c)*3+2],
			    255));
    }
  }
  delete [] GLpixmap;
  setBkgColor(old_col);
  return pm;
}
