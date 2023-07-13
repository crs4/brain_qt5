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
#ifndef bsCmdSectionEdit_h
#define bsCmdSectionEdit_h

#include "slCommand.h"
#include <sl/fixed_size_point.hpp>


const int STATUS_IDLE = 0;
const int STATUS_EDIT_VERTEX = 1;


class bsSectionEditTool;

class bsCmdSectionEdit: public slUndoableCommand {

public:

    bsCmdSectionEdit(bsSectionEditTool* tool, 
		     int status,
		     int current_section,
		     int current_deform_vertex,
		     const sl::point3f& position);

    const char *name() const;

    void redo();

    void undo();
 
protected:

    void backup();

protected:

    bsSectionEditTool* m_tool;
    int m_status;
    int m_current_section;
    int m_current_deform_vertex;

    sl::point3f m_position;
    sl::point3f m_old_position;

}; // bsCmdSectionEdit 

#endif
