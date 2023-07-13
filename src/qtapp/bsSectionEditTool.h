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
#ifndef BSSECTIONEDITTOOL_H
#define BSSECTIONEDITTOOL_H

#include "bsTool2D.h"
#include <sl/fixed_size_point.hpp>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QPixmap>


class bsEditor2D;
class bsViewer;
class bsSection;

class bsSectionEditTool: public bsTool2D {
    Q_OBJECT

public:

    bsSectionEditTool(bsEditor2D* editor,
		      QWidget* parent,
		      const char* name,
		      QPixmap* pixmap = NULL,
		      QBitmap* bitmask = NULL,
		      int cursor_type = C_CENTER);

    virtual ~bsSectionEditTool();


public slots: 

    virtual void activate(bool b);

    virtual void handleMousePress(QMouseEvent *e);
    virtual void handleMouseRelease(QMouseEvent *e);
    virtual void handleMouseMove(QMouseEvent *e);
    virtual void handleMouseEnter(QEvent *e);

    virtual void handleKeyPress(QKeyEvent *e);
    virtual void handleKeyRelease(QKeyEvent *e);

public:

    virtual void paintGL();

public:

    void execEdit(int section, int vertex, const sl::point3f position);
    sl::point3f currentPosition(int section, int vertex) const;
    const bsSection* currentSection();


protected:
    

    int         m_status;
    sl::point3f m_position;
    int         m_current_deform_vertex;
    int         m_current_section;
    int         m_key;
    bsViewer*   m_viewer;
};

#endif
