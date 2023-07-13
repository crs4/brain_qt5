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
#include "bsCmdAssignSubRegion.h"
#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsDocument.h"

bsCmdAssignSubRegion::bsCmdAssignSubRegion(bsDocument* doc, int section, int i, int j, int id) {
    m_doc = doc;
    m_section = section;
    m_i = i;
    m_j = j;
    m_id = id;
    m_old_id = -1;
}

const char* bsCmdAssignSubRegion::name() const {
    return "Set SubReg Id";
}  

void bsCmdAssignSubRegion::redo() {
    m_doc->setRegionId(m_section,m_i,m_j,m_id);
}

void bsCmdAssignSubRegion::undo() {
    m_doc->setRegionId(m_section,m_i,m_j,m_old_id);
}

void bsCmdAssignSubRegion::backup() {
    bsSection* s = m_doc->getBrain2D()->sectionItem(m_section);
    m_old_id = s->getRegionId(m_i,m_j);
}

