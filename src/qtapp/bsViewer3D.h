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

#include "bsViewer.h"
//Added by qt3to4:
#include <QWheelEvent>

class qtCommandHistory;
class bsGLPainter;

class bsViewer3D : public bsViewer
{
  Q_OBJECT

public:

  bsViewer3D( QWidget* parent = 0);

  virtual ~bsViewer3D();

  // Geometries
  float maxCameraDistance() const;
  float cameraFar() const;
  float cameraNear() const;
  float worldToPixel() const;

  // A P V Matrix  
  virtual sl::affine_map3f cameraV() const;
  virtual sl::projective_map3f cameraP() const;
  virtual sl::affine_map3f cameraA() const;
  virtual sl::projective_map3f cameraAP() const;

  // GL Functions
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL( int w, int h );
 

  // Command processor
  void setCommandProcessor(qtCommandHistory* cmd_processor);
  qtCommandHistory* commandProcessor();

  // bounding box
  inline void setInitBox(const sl::aabox3f& b) {
    m_init_box=b;
  }

  inline sl::aabox3f initBox() const{
    return m_init_box;
  }

  inline float scaleFactor() const {
    return m_current_zoom;
  }

  inline float aspectRatio() const {
    return m_current_aspect_ratio;
  }


public slots:

  void setCameraP(const sl::projective_map3f& P);
  void setCameraV(const sl::affine_map3f& V);
  void setCameraA(const float zoom);
  void resetP();
  void resetV();
  void resetA();
  void reset();
  void reset_right();
  void reset_left();
  void reset_top();
  void reset_bottom();
  void reset_rear();
  void handlingMouseWheelChangedEvent ( QWheelEvent * e );

signals:
  void matrixPChanged();
  void matrixVChanged();
  
protected:

  sl::aabox3f m_init_box;

  sl::projective_map3f m_current_cameraP;
  sl::affine_map3f m_current_cameraV;
  sl::affine_map3f m_current_cameraA;

  qtCommandHistory* m_cmd_processor;
  float m_current_zoom;
  float m_current_aspect_ratio;

};


#endif // _BSVIEWER3D_H








