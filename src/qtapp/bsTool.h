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
#ifndef BSTOOL_H
#define BSTOOL_H

#include <qobject.h>
//Added by qt3to4:
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QToolButton>
#include "bsGLPainter.h"
#include <sl/fixed_size_point.hpp>

const int C_CENTER = 0;
const int C_TOP_LEFT = 1;
const int C_TOP_RIGHT = 2;
const int C_BOTTOM_LEFT = 3;
const int C_BOTTOM_RIGHT = 4;

class QBitmap;
class QPixmap;
class QCursor;
class bsEditor;
class qtCommandHistory;
class QWidget;
class QAction;

class bsTool : public QObject, public bsGLPainter {
    
    Q_OBJECT

public:

    bsTool(bsEditor* editor,
	   QWidget* parent,
	   const char* name,
	   QPixmap* pixmap = NULL,
	   QBitmap* bitmask = NULL,
	   int cursor_type = C_CENTER);


    virtual ~bsTool();

    virtual inline QToolButton* getToolButton() {
	return m_tool_button;
    }

    inline QCursor* getToolCursor() {
	return m_cursor;
    }

    inline bool isActive() {
	return m_tool_active_status;
    }


public slots: 

    virtual void reset();
    virtual void setToolButton(bool b);
    
    void enableTool(bool b);
    virtual void activate(bool b);

    virtual void handleMousePress(QMouseEvent *e);
    virtual void handleMouseRelease(QMouseEvent *e);
    virtual void handleMouseMove(QMouseEvent *e);
    virtual void handleMouseDoubleClick(QMouseEvent *e);
    virtual void handleMouseEnter(QEvent *e);

    virtual void handleKeyPress(QKeyEvent *e);
    virtual void handleKeyRelease(QKeyEvent *e);

public:

    virtual void paintGL();

protected:

    virtual void updateGL();


protected: // Utility
    sl::point2f pixelsToNormalized(QMouseEvent *e) const; 
    sl::point2f worldToNormalized(const sl::point3f& p) const;
    sl::point3f normalizedToWorldNear(const sl::point2f& p) const;
    sl::point3f normalizedToWorldFar(const sl::point2f& p) const;


protected:
  QString m_tool_name;
  bool m_tool_active_status;
  QToolButton*      m_tool_button;
  QAction* m_tool_button_act;
  bsEditor* m_editor;
  qtCommandHistory* m_cmd_processor;
  QCursor* m_cursor;
};

#endif
