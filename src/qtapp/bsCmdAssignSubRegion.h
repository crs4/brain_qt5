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
#ifndef bsCmdAssignSubRegion_h
#define bsCmdAssignSubRegion_h

#include "slCommand.h"

class bsDocument;

class bsCmdAssignSubRegion: public slUndoableCommand {

public:

    bsCmdAssignSubRegion(bsDocument* doc, int section, int i, int j, int id);

    const char *name() const;

    void redo();

    void undo();
 
protected:

    void backup();

protected:

    bsDocument* m_doc;
    int m_section;
    int m_i;
    int m_j;
    int m_id;
    int m_old_id;

}; // bsCmdAssignSubRegion 

#endif
