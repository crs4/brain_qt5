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
#ifndef _BSVIEWER3D_H
#define _BSVIEWER3D_H

#include <qgl.h>
#include <sl/axis_aligned_box.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>
#include <vector>

class bsGLPainter;

class bsViewer3D : public QGLWidget
{
  Q_OBJECT

public:

  bsViewer3D( QWidget* parent = NULL, 
	      const char* name = NULL, 
	      const QGLWidget* shareWidget = NULL );
  virtual ~bsViewer3D();
  
  sl::affine_map3f cameraV() const;
  sl::projective_map3f cameraP() const;
  sl::projective_map3f cameraPV() const;
  sl::projective_map3f cameraAP() const;

  inline sl::aabox3f initBox() const {
    return m_init_box;
  }

  inline void setInitBox(const sl::aabox3f& b) {
    m_init_box=b;
    reset();
  }


  // Painting

  void registerPainter(bsGLPainter* p);
  void unregisterPainter(bsGLPainter* p);

public slots:

  void setCameraP(const sl::projective_map3f& P);
  void setCameraV(const sl::affine_map3f& V);

signals:
  void matrixPChanged();
  void matrixVChanged();

  void mousePress(QMouseEvent*);
  void mouseRelease(QMouseEvent*);
  void mouseMove(QMouseEvent*);
  void mouseDoubleClick(QMouseEvent*);

public:

  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );
  
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL( int w, int h );

protected:

  virtual void paintPainters();

protected:
  
  sl::aabox3f m_init_box;
  sl::projective_map3f m_current_cameraP;
  sl::affine_map3f m_current_cameraV;
  sl::affine_map3f m_current_cameraA;

  std::vector<bsGLPainter*> m_painters;
};


#endif // _BSVIEWER3D_H








