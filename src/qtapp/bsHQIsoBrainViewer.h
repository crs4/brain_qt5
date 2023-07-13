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
#ifndef _BSHQISOBRAINVIEWER_H
#define _BSHQISOBRAINVIEWER_H

#include <qgl.h>

#include <sl/axis_aligned_box.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>

#include "bsBrainViewer.h"

class bsDocument;
class bsRegion;
class bsCortexRegion;
class bsSectionedBrain;
class qtCommandHistory;

class mc_coded_isosurface;

typedef sl::fixed_size_vector<sl::column_orientation,3,unsigned char> sl_color3ub;


class bsHQIsoBrainViewer : public bsBrainViewer {
  Q_OBJECT

public:

  bsHQIsoBrainViewer (QWidget* parent = 0);

  virtual ~bsHQIsoBrainViewer();

public:

  virtual void localPaintGL();
  virtual void setDocument(bsDocument* doc);


public slots:
 
  virtual void rebuild();

  virtual void handleBrainChange();

  virtual void handleSectionChange(int i);

  virtual void setEnableSubRegions(bool b);

  void setCuttingBoxFromDocument();
    
protected:

  void iso_clear();

  void iso_repair();

  void iso_rebuild();

  void iso_render();

  void section_draw_coded_red(int i);

  void render_region(const bsRegion* cr, int region_i);
  void triangle_render(const bsRegion* cr, int region_i);
  void render_boundaries(const bsCortexRegion* cr);

  void iso_recompute_normals();
  void iso_relax();

protected:

  bool m_enable_subregions;

  bsSectionedBrain *m_brain;

  mc_coded_isosurface *m_iso_geometry;

};


#endif // _BSHQISOBRAINVIEWER_H

