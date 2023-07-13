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
#include "bsCmdUpDownSection.h"
#include "bsSectionViewer2D.h"

bsCmdUpDownSection::bsCmdUpDownSection(bsSectionViewer2D* v,
			   const char motion_type,
			   int value) {
  m_viewer = v;
  m_type = motion_type;
  m_count_section=value;
  m_old_up_count_section=0;
  m_old_down_count_section=0;
}

const char* bsCmdUpDownSection::name() const {
  switch (m_type) {
  case 'u': return "Up Section"; break;
  case 'd': return "Down Section"; break;
  }
  return NULL;
}  

void bsCmdUpDownSection::redo() {
  switch (m_type) {
  case 'u': m_viewer->setUpCountSectionCmd(m_count_section); break;
  case 'd': m_viewer->setDownCountSectionCmd(m_count_section); break;
  }
}

void bsCmdUpDownSection::undo() {
  switch (m_type) {
  case 'u': 
    m_viewer->setUpCountSectionCmd(m_old_up_count_section); 
    break;
  case 'd': 
    m_viewer->setDownCountSectionCmd(m_old_down_count_section); 
    break;
  }
}

void bsCmdUpDownSection::backup() {
  m_old_up_count_section = m_viewer->upCountSection();
  m_old_down_count_section = m_viewer->downCountSection();
}
