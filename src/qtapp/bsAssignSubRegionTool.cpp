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
#include "bsAssignSubRegionTool.h"
#include "bsAssignWidget.h"

#include "bsSectionedBrain.h"
#include "bsCortexRegion.h"
#include "bsSection.h"

#include "bsEditor2D.h"

#include "bsCmdAssignSubRegion.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>
#include "bsDocument.h"
#include <assert.h>

bsAssignSubRegionTool::bsAssignSubRegionTool(bsEditor2D* editor,
					   QWidget* parent,
					   const char* name,
					   QPixmap* pixmap,
					   QBitmap* bitmask):
    bsTool2D(editor, parent, name, pixmap, bitmask) {
}

bsAssignSubRegionTool::~bsAssignSubRegionTool() {
}


int bsAssignSubRegionTool::getIdDialog(int id) const {
    bsDocument* doc = m_editor->getDocument();
    int result = -1;
    if (doc) {
	bsSectionedBrain* b = doc->getBrain2D();
	bsAssignWidget* assign_window = new bsAssignWidget;
	for (int i=0; i<b->regionLabelCount(); i++) {
	  assign_window->setItem(b->regionLabelItem(i).c_str());
	    if (i == id) {
		assign_window->setCurrentItem(id);
	    }
	}
	result=assign_window->exec();
	delete assign_window;
    }
    return result;
}


void bsAssignSubRegionTool::setSubRegionId(bsDocument* doc, int section, int i, int j, int id) {
    bsCmdAssignSubRegion * cmd = new bsCmdAssignSubRegion(doc, section, i, j, id);
    m_cmd_processor->doit(cmd);
}


void bsAssignSubRegionTool::handleMousePress(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
	sl::point2f pn_mouse  = pixelsToNormalized(e);
	m_start_point = normalizedToWorldNear(pn_mouse);
	assert(m_editor);
	bsDocument* doc = m_editor->getDocument();
	int current_section = editor2D()->getCurrentSectionIndex();
	if (doc) {
	    bsSectionedBrain* b = doc->getBrain2D();
	    if (b->goodSectionIndex(current_section)) {
		bsSection* s = b->sectionItem(current_section);
		sl::tuple2i ij=s->getRegion(sl::point2f(m_start_point[0],m_start_point[1]));
		if (ij[0]>=0 && ij[1]>=0) {
		    int old_id = s->getRegionId(ij[0],ij[1]);
		    int id = getIdDialog(old_id);
		    if (id >= 0) {
			setSubRegionId(doc,current_section,ij[0],ij[1],id);
		    }
		} 
	    }
	}

    } 
}
