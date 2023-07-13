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
#ifndef BSCUTTINGBOXTOOL_H
#define BSCUTTINGBOXTOOL_H

#include "bsTool3D.h"
#include <sl/fixed_size_point.hpp>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QPixmap>

class bsBrainViewer;
class bsCuttingBoxWidgetImpl;

class bsCuttingBoxTool: public bsTool3D {
    Q_OBJECT

public:

    bsCuttingBoxTool(bsEditor3D* editor,
			QWidget* parent,
			const char* name,
			QPixmap* pixmap = NULL,
			QBitmap* bitmask = NULL);

    virtual ~bsCuttingBoxTool();

public slots: 

    virtual void activate(bool b);

    virtual void handleMousePress(QMouseEvent *e);
    virtual void handleMouseRelease(QMouseEvent *e);
    virtual void handleMouseMove(QMouseEvent *e);
    virtual void handleMouseEnter(QEvent *e);

    virtual void handleKeyPress(QKeyEvent *e);
    virtual void handleKeyRelease(QKeyEvent *e);

    void changeLengths();
    void updateCuttingBoxWindow();
    void resetCuttingBox();
    void traslateIJKCuttingBox();

signals:
    void emitArbitrarySlicing(bool);
    void cuttingBoxChanged();

protected:
    int       m_status;
    sl::point3f m_start_point;
    sl::point3f m_end_point;
    //    sl::point3f m_old_trasl_ijk_value;
 
    bsBrainViewer* m_viewer;
    int m_key;
    bsCuttingBoxWidgetImpl* m_cutting_box_window;
};

#endif //BSCUTTINGBOXTOOL_H
