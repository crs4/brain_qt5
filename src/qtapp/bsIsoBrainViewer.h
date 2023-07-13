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
#ifndef _BSISOBRAINVIEWER_H
#define _BSISOBRAINVIEWER_H

#include <qgl.h>

#include <sl/axis_aligned_box.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>

#include "bsBrainViewer.h"

class bsDocument;
class bsRegion;
class bsSectionedBrain;
class bsSectionRenderer;
class qtCommandHistory;

class mc_coded_isosurface;

typedef sl::fixed_size_vector<sl::column_orientation,3,unsigned char> sl_color3ub;


class bsIsoBrainViewer : public bsBrainViewer {
  Q_OBJECT

public:

  bsIsoBrainViewer (QWidget* parent = NULL, 
		    const char* name = NULL,
		    const QGLWidget* shareWidget = NULL);

  virtual ~bsIsoBrainViewer();

public:

  virtual void localPaintGL();


public slots:
 
  virtual void rebuild();

  virtual void handleBrainChange();

  virtual void handleSectionChange(int i);

  virtual void setEnableSubRegions(bool b);
    
protected:

  void iso_init();

  void iso_clear();

  void iso_damage(int i);

  void iso_repair();

  void iso_rebuild(int i);

  void iso_render(int i);

  void section_draw_coded_red(int i);

  void render_region(const bsRegion* parsed_region, int region_i);
  void triangle_render(const bsRegion* parsed_region, int region_i);
  void tessel_render(const bsRegion* parsed_region, int region_i);


protected:

  bool m_enable_subregions;

  bsSectionedBrain *m_brain;

  std::vector<struct mc_coded_isosurface *>     m_iso_slices;

};


#endif // _BSISOBRAINVIEWER_H

