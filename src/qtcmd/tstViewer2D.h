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
#ifndef _BSVIEWER2D_H
#define _BSVIEWER2D_H

#include <qgl.h>
#include <sl/linear_map_factory.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>
#include <sl/axis_aligned_box.hpp>

class tstViewer2D : public QGLWidget
{
  Q_OBJECT

public:

  tstViewer2D( QWidget* parent = NULL, 
	      const char* name = NULL, 
	      const QGLWidget* shareWidget = NULL );
  virtual ~tstViewer2D();
  
  sl::affine_map3f cameraV() const;
  sl::projective_map3f cameraP() const;

  int getZoomFactor() const;
  int getXTranslation() const;
  int getYTranslation() const;

  inline sl::aabox3f initBox() const {
    return m_init_box;
  }

  inline void setInitBox(const sl::aabox3f& b) {
    m_init_box=b;
    reset();
  }

public slots:
 
  void setXTranslation( int step );
  void setYTranslation( int step );
  void setZoomFactor( int factor);
  void reset();
  
signals:

  void xTranslationChanged(int);
  void yTranslationChanged(int);
  void zoomFactorChanged(int);

protected:
  
  void		initializeGL();
  void		paintGL();
  void		resizeGL( int w, int h );
  
protected:
  
  sl::aabox3f m_init_box;
  float m_current_zoom_factor;
  sl::column_vector2f m_current_translation;
  
};


#endif // _BSVIEWER2D_H
