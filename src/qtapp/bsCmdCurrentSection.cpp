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
#include "bsCmdCurrentSection.h"
#include "bsSectionViewer2D.h"

bsCmdCurrentSection::bsCmdCurrentSection(bsSectionViewer2D* v, int n_sect) {
  m_viewer = v;
  m_section=n_sect;
  m_old_section = -1;
}

const char* bsCmdCurrentSection::name() const {
    return "Set section";
}  


void bsCmdCurrentSection::redo() {
 m_viewer->setCurrentSectionCmd(m_section);
}

void bsCmdCurrentSection::undo() {
 m_viewer->setCurrentSectionCmd(m_old_section);
}

void bsCmdCurrentSection::backup() {
  m_old_section = m_viewer->currentSection();
}

