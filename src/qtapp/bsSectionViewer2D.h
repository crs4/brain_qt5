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
#ifndef _BSSECTIONVIEWER2D_H
#define _BSSECTIONVIEWER2D_H

#include <qgl.h>

#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>
#include <sl/axis_aligned_box.hpp>

#include "bsViewer2D.h"

class bsDocument;
class bsSectionRenderer;
class qtCommandHistory;
class bsTexViewer;

class bsSectionViewer2D : public bsViewer2D {
  Q_OBJECT

public:

  bsSectionViewer2D (QWidget* parent = 0);

  virtual ~bsSectionViewer2D();


public:

  virtual void paintGL();

  void setCurrentSectionCmd(int n_sect);
  void setUpCountSectionCmd(int up_sect);
  void setDownCountSectionCmd(int down_sect);
  void dummy(); 
  
  inline int currentSection() const {
      return m_current_section;
  }

  inline int upCountSection() const {
      return m_up_count_section;
  }

  inline int downCountSection() const {
      return m_down_count_section;
  }
 
  void setDocument(bsDocument* doc);

  bool cuttingPlanesEnabled()
 const {
    return m_cutting_planes_flag;
  }
  
 public slots:

  void setCurrentSection( int n_sect );
  void setUpCountSection(int up_sect);
  void setDownCountSection(int down_sect);
  void setupRenderer2D();
  void redrawOnChange();
  void fillSection(bool flag);
  void setupRegionOptions();
  void setupNeuronTypeOptions();
  void setCuttingPlanes(bool b);
  void loadImage();
  void loadMultiImage();
  void enableImageDrawing(bool b);
  void enableMask(bool b);
  void setThreshold(int th);

signals:

  void currentSectionChanged(int);
  void upCountSectionChanged(int);
  void downCountSectionChanged(int);
  void enableUpCounter(bool);
  void enableDownCounter(bool);

protected:
  void drawCuttingBox();
  float intersection(sl::point3f& p0, sl::point3f& p1, float z);
  void sortIntersectionList(std::vector<sl::point3f>& plist);
  float angle(sl::vector3f& u, sl::vector3f& v);

protected:
  
  bsSectionRenderer* m_render;
  bsSectionRenderer* m_up_render;
  bsSectionRenderer* m_down_render;
  
  bsDocument* m_document;
  int m_current_section;
  int m_up_count_section;
  int m_down_count_section;
  
  float m_neuronsize;
  float m_bordersize;


  bsTexViewer* m_background_render;

  bool m_cutting_planes_flag;
  bool m_image_drawing_flag;
  bool m_mask_drawing_flag;
};


#endif // _BSSECTIONVIEWER2D_H
