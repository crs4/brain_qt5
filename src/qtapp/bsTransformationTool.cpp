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
#include "bsTransformationTool.h"
#include "bsEditor2D.h"
#include "bsCmdTransformSection.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QPixmap>
#include "bsDocument.h"
#include <assert.h>

bsTransformationTool::bsTransformationTool(bsEditor2D* editor,
					   QWidget* parent,
					   const char* name,
					   QPixmap* pixmap,
					   QBitmap* bitmask):
    bsTool2D(editor, parent, name, pixmap, bitmask) {
}

bsTransformationTool::~bsTransformationTool() {

}

void bsTransformationTool::transformSection(const char t_type, 
					    const sl::affine_map2f& t) {
      bsCmdTransformSection * cmd = new bsCmdTransformSection(this, t_type, t);
      m_cmd_processor->doit(cmd);
}

void bsTransformationTool::transformSectionCmd(const sl::affine_map2f& t){

    bsDocument* doc = m_editor->getDocument();
    int current_section = editor2D()->getCurrentSectionIndex();

    assert(doc);
    assert(current_section >= 0);

    doc->transformSection(current_section,t);
}

