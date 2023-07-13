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
#ifndef _BSFASTBRAINVIEWER_H
#define _BSFASTBRAINVIEWER_H

#include <qgl.h>

#include <sl/axis_aligned_box.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>

#include "bsBrainViewer.h"


class bsDocument;
class bsSectionRenderer;
class qtCommandHistory;


class bsFastBrainViewer : public bsBrainViewer {
  Q_OBJECT

public:

  bsFastBrainViewer (QWidget* parent = 0);

  virtual ~bsFastBrainViewer();


public: // Commands

  virtual void localPaintGL();

public slots:

  virtual void handleBrainChange();

  virtual void setSelectedRendererIndex(int n);

  virtual void handleSectionChange(int);
  virtual void setStepSize(int value);
  virtual void setDraftQuality(bool b);

  virtual void setEnableSubRegions(bool b);
  virtual void setEnableNeuronTypes(bool b);
  virtual void setEnableSelectedRenderer(bool b);

protected:
  
  bsSectionRenderer* m_selected_render;
  int m_selected_renderer_index;
  bool m_enable_selected_renderer;

  bool m_draft_quality;

};


#endif // _BSFASTBRAINVIEWER_H
