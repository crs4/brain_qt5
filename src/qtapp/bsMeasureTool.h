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
#ifndef BSMEASURETOOL_H
#define BSMEASURETOOL_H

#include "bsTool2D.h"
#include <sl/fixed_size_point.hpp>
//Added by qt3to4:
#include <QMouseEvent>
#include <QPixmap>

class bsMeasureWidgetImpl;
class bsEditor2D;

class bsMeasureTool: public bsTool2D {
    Q_OBJECT

public:

    bsMeasureTool(bsEditor2D* editor,
		  QWidget* parent,
		  const char* name,
		  QPixmap* pixmap = NULL,
		  QBitmap* bitmask = NULL,
		  int cursor_type = C_CENTER);

    virtual ~bsMeasureTool();


public slots: 

    virtual void activate(bool b);

    virtual void handleMousePress(QMouseEvent *e);
    virtual void handleMouseRelease(QMouseEvent *e);
    virtual void handleMouseMove(QMouseEvent *e);


public:

    virtual void paintGL();

protected:
    void updateMeasureWindow();
  
protected:

    int       m_status;
    float     m_center_radius;

    sl::point3f m_end_point;
    sl::point3f m_start_point;

    bsMeasureWidgetImpl* m_measure_window;
};

#endif
