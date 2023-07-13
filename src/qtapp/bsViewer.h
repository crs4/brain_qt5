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
#ifndef _BSVIEWER_H
#define _BSVIEWER_H

#include <qgl.h>
//Added by qt3to4:
#include <QWheelEvent>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <sl/linear_map_factory.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>
#include <sl/axis_aligned_box.hpp>
#include <sl/fixed_size_point.hpp>
#include <vector>

class bsGLPainter;
class QImage;

class bsViewer : public QGLWidget
{
  Q_OBJECT

public:

  bsViewer( QWidget* parent = 0);
  virtual ~bsViewer();
  
  virtual sl::affine_map3f cameraV() const = 0;
  virtual sl::projective_map3f cameraP() const = 0;
  sl::projective_map3f cameraPV() const;


  // Painting

  void registerPainter(bsGLPainter* p);
  void unregisterPainter(bsGLPainter* p);

  // Utility
  sl::point2f pixelsToNormalized(QMouseEvent *e) const; 
  sl::point2f worldToNormalized(const sl::point3f& p) const;
  sl::point3f normalizedToWorldNear(const sl::point2f& p) const;
  sl::point3f normalizedToWorldFar(const sl::point2f& p) const;
  sl::point3f worldToCamera(const sl::point3f& p) const;
  

  QImage getGLPixmap();

signals:
  void mousePress(QMouseEvent*);
  void mouseRelease(QMouseEvent*);
  void mouseMove(QMouseEvent*);
  void mouseDoubleClick(QMouseEvent*);
  void keyPress(QKeyEvent*);
  void keyRelease(QKeyEvent*);
  void mouseEnter(QEvent*);
  void mouseWheelChanged(QWheelEvent *);
  
public:
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  void enterEvent(QEvent *e);
  void wheelEvent ( QWheelEvent * e ) ;
  void keyPressEvent(QKeyEvent* e);
  void keyReleaseEvent(QKeyEvent* e);

  virtual void initializeGL() = 0;
  virtual void paintGL() = 0;
  virtual void resizeGL( int w, int h ) = 0;

  bool fancyRenderingEnabled() const {
    return m_fancy_rendering_flag;
  }

  void setFancyRendering(bool b) {
    m_fancy_rendering_flag = b;
  }

  inline sl::color3f bkgColor() const {
    return m_bkg_color;
  }
  
  inline void setBkgColor(const sl::color3f& c) {
    m_bkg_color = c;
  }


protected:

  virtual void paintPainters();

protected:
  std::vector<bsGLPainter*> m_painters;
  sl::color3f m_bkg_color;
  bool m_fancy_rendering_flag;
};


#endif // _BSVIEWER_H








