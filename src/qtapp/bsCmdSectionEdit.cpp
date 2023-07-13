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
#include "bsCmdSectionEdit.h"
#include "bsSectionEditTool.h"

#include <iostream>

bsCmdSectionEdit::bsCmdSectionEdit(bsSectionEditTool* tool,
				   int status,
				   int current_section,
				   int current_deform_vertex,
				   const sl::point3f& position) {
  m_tool = tool;
  m_status = status;
  m_current_section = current_section;
  m_current_deform_vertex = current_deform_vertex;
  m_position = position;
  m_old_position=m_tool->currentPosition(m_current_section, m_current_deform_vertex);
}

const char* bsCmdSectionEdit::name() const {
    switch (m_status) {
      case STATUS_EDIT_VERTEX: return "Section Edit"; break;
    }
    return NULL;
}  


void bsCmdSectionEdit::redo() {
  m_tool->execEdit(m_current_section,m_current_deform_vertex,m_position);
}

void bsCmdSectionEdit::undo() {
     m_tool->execEdit(m_current_section,m_current_deform_vertex,m_old_position);
}

void bsCmdSectionEdit::backup() {
    m_old_position=m_tool->currentPosition(m_current_section, m_current_deform_vertex);
}




