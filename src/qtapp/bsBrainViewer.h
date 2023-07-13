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
#ifndef _BSBRAINVIEWER_H
#define _BSBRAINVIEWER_H

#include <qgl.h>

#include <sl/axis_aligned_box.hpp>
#include <sl/oriented_box.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>

#include "bsViewer3D.h"

class bsDocument;
class bsSectionRenderer;
class qtCommandHistory;


class bsBrainViewer : public bsViewer3D {
  Q_OBJECT

public:

  bsBrainViewer (QWidget* parent = 0);

  virtual ~bsBrainViewer();


public: // Commands

  virtual void paintGL();
  virtual void localPaintGL() = 0;
  virtual void cutPaintGL();
  virtual void capsPaintGL();
  virtual void enableCutPlanesExcept(int j);

  virtual void setDocument(bsDocument* doc);

public: // Queries

  bool cuttingPlanesEnabled() const {
    return m_cutting_planes_flag;
  }

  bool cuttingCapsEnabled() const {
    return m_cutting_caps_flag;
  }

  bool cuttingToolEnabled() const {
    return m_cutting_tool_flag;
  }

  bool lightTwoSideEnabled() const {
    return m_light_two_side;
  }
  
  void setCuttingToolEnabled(bool b) {
    m_cutting_tool_flag = b;
  }
  
  sl::obox3f getCuttingBox() const {
    return m_cutting_box;
  }

  sl::aabox3f getBoundingBox() const;
  
  bsDocument* getDocument() const ;
  bsSectionRenderer* getRenderer() const;

  virtual float getExtinctionZetaValue() const;
  virtual float getExtinctionZetaThresholdStart() const;
  virtual float getExtinctionZetaThresholdEnd() const;

public slots:

  virtual void handleBrainChange() = 0;

  virtual void handleSectionChange(int) = 0;

  virtual void setEnableSubRegions(bool b) = 0;
  virtual void setEnableNeuronTypes(bool b);

  virtual void setupRegionOptions();
  virtual void setupNeuronTypeOptions();

  virtual void handleExtinctionZetaChange(int);
  virtual void handleExtinctionZetaThresholdChangeStart(int);
  virtual void handleExtinctionZetaThresholdChangeEnd(int);

  virtual void setCuttingPlanesFlag(bool b);
  virtual void setCuttingCapsFlag(bool b);
  virtual void setLightTwoSide(bool b);

  virtual void setCuttingBox(const sl::obox3f& box);
  void resetCuttingBox();

signals:
  void cuttingPlanesChanged(sl::obox3f&);
  void newBrain2D(bool);

protected:
  void drawPlane(int i);
  void drawCuttingBox();

protected:
  
  bsSectionRenderer* m_render;

  bsDocument* m_document;

  float m_extinction_zeta;
  int m_extinction_zeta_threshold[2];

  bool m_cutting_planes_flag;
  bool m_cutting_caps_flag;
  bool m_cutting_tool_flag;
  bool m_light_two_side;
  sl::obox3f m_cutting_box;

};


#endif // _BSBRAINVIEWER_H
