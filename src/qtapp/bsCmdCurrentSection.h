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
#ifndef bsCmdCurrentSection_h
#define bsCmdCurrentSection_h

#include "slCommand.h"

class bsSectionViewer2D;

class bsCmdCurrentSection: public slUndoableCommand {

public:

    bsCmdCurrentSection(bsSectionViewer2D* v, int n_sect);

    const char *name() const;

    void redo();

    void undo();

    inline int get_m_section(){return m_section;}
 
protected:

    void backup();

protected:

    int  m_section;
    bsSectionViewer2D* m_viewer;
    int m_old_section;

}; // bsCmdCurrentSection 

#endif
