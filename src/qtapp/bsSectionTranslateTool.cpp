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
#include "bsSectionTranslateTool.h"
#include "bsSectionedBrain.h"
#include "bsSectionViewer2D.h"
#include "bsEditor2D.h"
#include "bsDocument.h"
#include <sl/affine_map.hpp>
#include <GL/gl.h>
#include <math.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QPixmap>


bsSectionTranslateTool::bsSectionTranslateTool(bsEditor2D* editor,
					       QWidget* parent,
					       const char* name,
					       QPixmap* pixmap,
					       QBitmap* bitmask):
    bsTransformationTool(editor, parent, name, pixmap, bitmask) {
}


bsSectionTranslateTool::~bsSectionTranslateTool() {
}


void bsSectionTranslateTool::activate(bool b) {
    bsTool::activate(b);
}


void bsSectionTranslateTool::handleMousePress(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
	sl::point2f pn_mouse  = pixelsToNormalized(e);
	m_start_point = normalizedToWorldNear(pn_mouse);
    } 
}


void bsSectionTranslateTool::handleMouseRelease(QMouseEvent *) {
}


void bsSectionTranslateTool::handleMouseMove(QMouseEvent *e) {
    sl::point2f pn_mouse = pixelsToNormalized(e);
    sl::point3f p_current = normalizedToWorldNear(pn_mouse);
    sl::column_vector3f t = p_current - m_start_point;
    sl::affine_map2f T = sl::linear_map_factory2f::translation(t[0],t[1]);
    transformSection('t',T);
    m_start_point=p_current;
}

