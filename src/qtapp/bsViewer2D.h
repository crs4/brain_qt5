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

#include "bsViewer.h"

class qtCommandHistory;
class bsGLPainter;

class bsViewer2D : public bsViewer
{
  Q_OBJECT

public:

  bsViewer2D( QWidget* parent = 0);

  virtual ~bsViewer2D();
  
  virtual sl::affine_map3f cameraV() const;
  virtual sl::projective_map3f cameraP() const;

  float zoomFactor(int i) const;

  float worldToPixel() const;

  inline void setInitBox(const sl::aabox3f& b) {
    m_init_box=b;
    reset();
  }

  // Pixel 
  int getZoomFactor() const;
  int getXTranslation() const;
  int getYTranslation() const;

  // Normalized Pixel
  float getNormalizedZoomFactor() const;
  float getNormalizedXTranslation() const;
  float getNormalizedYTranslation() const;


public slots:

  void setCellStepX(float value);
  void setCellStepY(float value);

  void setGrid(bool b);

  void setZoomFactor(int factor);
  void setXTranslation(int xfactor);
  void setYTranslation(int yfactor);
  void reset();
  
  void undo();
  void redo();

public:
 
  void setXTranslationCmd(float tx);
  void setYTranslationCmd(float ty);
  void setZoomFactorCmd(float zf);
  void resetCmd();
  
signals:

  void xTranslationChanged(int);
  void yTranslationChanged(int);
  void zoomFactorChanged(int);

public:

  virtual void initializeGL();
  virtual void paintGL();
  virtual void drawGrid();
  virtual void resizeGL( int w, int h );

  void setCommandProcessor(qtCommandHistory* cmd_processor);
  qtCommandHistory* commandProcessor();

protected:
  float normalizedTranslation(int i, int step) const;
  float normalizedZoomFactor(int factor) const;

  
protected:
  sl::aabox3f m_init_box;
  float m_current_zoom_factor;
  sl::column_vector2f m_current_translation;
  float m_current_aspect_ratio;
  qtCommandHistory* m_cmd_processor;
  bool m_grid_flag;
  std::vector<bsGLPainter*> m_painters;

  float m_cell_stepX;
  float m_cell_stepY;
};


#endif // _BSVIEWER2D_H








