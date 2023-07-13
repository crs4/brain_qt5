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
#ifndef bsCmdUpDownSection_h
#define bsCmdUpDownSection_h

#include "slCommand.h"

class bsSectionViewer2D;

class bsCmdUpDownSection: public slUndoableCommand {
public:

    bsCmdUpDownSection(bsSectionViewer2D* v,
		 const char motion_type,
		 int value);

    const char *name() const;

    void redo();

    void undo();

protected:

    void backup();

protected:

    char m_type;
    int m_count_section;
    bsSectionViewer2D* m_viewer;

    int m_old_up_count_section;
    int m_old_down_count_section;

}; // bsCmdUpDownSection 

#endif
