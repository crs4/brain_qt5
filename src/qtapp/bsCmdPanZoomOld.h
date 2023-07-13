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
#ifndef bsCmdPanZoom_h
#define bsCmdPanZoom_h

#include "slCommand.h"

class bsViewer2D;

class bsCmdPanZoom: public slUndoableCommand {
public:
    bsCmdPanZoom(bsViewer2D* v,
		 const char motion_type,
		 int motion_value);

    const char *name() const;

    void redo();

    void undo();
 
    virtual bool isComposable( slCommand *c ) const;
    
    virtual void composeAndDestroy( slCommand *c );

protected:

    void backup();

protected:

    char m_type;
    int  m_value;
    bsViewer2D* m_viewer;

    int m_old_zoom_value;
    int m_old_x_value;
    int m_old_y_value;
}; // bsCmdPanZoom 

#endif