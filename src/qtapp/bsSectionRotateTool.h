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
#ifndef BSSECTIONROTATETOOL_H
#define BSSECTIONROTATETOOL_H

#include "bsTransformationTool.h"
#include <sl/fixed_size_point.hpp>
//Added by qt3to4:
#include <QMouseEvent>
#include <QPixmap>

class bsSectionRotateTool: public bsTransformationTool {
    Q_OBJECT

public:

    bsSectionRotateTool(bsEditor2D* editor,
			QWidget* parent,
			const char* name,
			QPixmap* pixmap = NULL,
			QBitmap* bitmask = NULL);

    virtual ~bsSectionRotateTool();


public slots: 

    virtual void activate(bool b);

    void resetCenter();

    virtual void handleMousePress(QMouseEvent *e);
    virtual void handleMouseRelease(QMouseEvent *e);
    virtual void handleMouseMove(QMouseEvent *e);


public:

    virtual void paintGL();


protected:

    int       m_status;
    sl::point3f m_center;
    float     m_center_radius;
    sl::point3f m_start_point;
};

#endif
