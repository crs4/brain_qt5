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
#include "bsTool.h"
#include "bsEditor.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>
#include "bsViewer.h"
#include "bsDocument.h"

#include <qtoolbutton.h>
#include <qbitmap.h>
#include <qpixmap.h>
#include <qcursor.h>
#include <qtooltip.h>

#include <assert.h>
#include <iostream>


bsTool::bsTool(bsEditor* editor,
	       QWidget* parent,
	       const char* name,
	       QPixmap* pixmap,
	       QBitmap* bitmask,
	       int cursor_type): QObject(parent) {

    assert(name);
    assert(editor);
    m_tool_name = name;
    m_tool_button = new QToolButton;
    m_tool_button_act = new QAction;
    m_tool_button->setDefaultAction(m_tool_button_act);
    m_tool_button_act->setToolTip(tr(name));
    m_tool_button_act->setCheckable(true);    
    m_tool_active_status=false;
    if (pixmap) {
	assert(bitmask);
	pixmap->setMask(*bitmask);
	int hotX=-1;
	int hotY=-1;
	switch (cursor_type) {
	case C_CENTER: 
	    hotX=-1;
	    hotY=-1;
	    break;
	case C_TOP_LEFT: 
	    hotX=0;
	    hotY=0;
	    break;
	case C_TOP_RIGHT: 
	    hotX=pixmap->width();
	    hotY=0;
	    break;
	case C_BOTTOM_LEFT: 
	    hotX=0;
	    hotY=pixmap->height();
	    break;
	case C_BOTTOM_RIGHT: 
	    hotX=pixmap->width();
	    hotY=pixmap->height();
	    break;
	}
	m_cursor = new QCursor(*pixmap,hotX,hotY);
        m_tool_button_act->setIcon(*pixmap);
    } else {
      m_cursor = new QCursor(Qt::ArrowCursor);
	char acr_name[2];
	acr_name[0]=name[0];
	acr_name[1]='\0';
	m_tool_button_act->setText(tr(acr_name));
    }

    m_editor = editor;
    m_cmd_processor = NULL;
    connect(m_tool_button_act, SIGNAL(toggled(bool)), this, SLOT(activate(bool))); }

bsTool::~bsTool() {
}


void bsTool::activate(bool b) {
#ifndef NDEBUG
  std::cerr << "#################### " << m_tool_name.toStdString() << " activate" << b << std::endl;
#endif
    m_tool_active_status=b;
    m_cmd_processor = m_editor->getCmdProcessor();
    if (b) {
	m_editor->setCurrentTool(this);
	m_editor->setViewCursor(*m_cursor);
	bsViewer* v= (bsViewer*)(m_editor->getViewer());
	connect(v, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(handleMousePress(QMouseEvent *)));
	connect(v, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(handleMouseRelease(QMouseEvent *)));
	connect(v, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(handleMouseMove(QMouseEvent *)));
	connect(v, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(handleMouseDoubleClick(QMouseEvent *)));
	connect(v, SIGNAL(mouseEnter(QEvent *)), this, SLOT(handleMouseEnter(QEvent *)));
	connect(v, SIGNAL(keyPress(QKeyEvent *)), this, SLOT(handleKeyPress(QKeyEvent *)));
	connect(v, SIGNAL(keyRelease(QKeyEvent *)), this, SLOT(handleKeyRelease(QKeyEvent *)));
	v->registerPainter(this);
    } else {
	bsViewer* v= (bsViewer*)(m_editor->getViewer());
	disconnect(v, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(handleMousePress(QMouseEvent *)));
	disconnect(v, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(handleMouseRelease(QMouseEvent *)));
	disconnect(v, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(handleMouseMove(QMouseEvent *)));
	disconnect(v, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(handleMouseDoubleClick(QMouseEvent *)));
	disconnect(v, SIGNAL(mouseEnter(QEvent *)), this, SLOT(handleMouseEnter(QEvent *)));
	disconnect(v, SIGNAL(keyPress(QKeyEvent *)), this, SLOT(handleKeyPress(QKeyEvent *)));
	disconnect(v, SIGNAL(keyRelease(QKeyEvent *)), this, SLOT(handleKeyRelease(QKeyEvent *)));
	v->unregisterPainter(this);
	if (m_editor->getCurrentTool() == this) {
	    m_editor->setCurrentTool(NULL);
	}
	m_editor->setViewCursor(QCursor(Qt::ArrowCursor));
    }
}

void bsTool::handleMousePress(QMouseEvent *) {
#ifndef NDEBUG
  //std::cerr << m_tool_button->name() << " handling mouse press." << std::endl;
#endif
}

void bsTool::handleMouseRelease(QMouseEvent *) {
#ifndef NDEBUG
  //std::cerr << m_tool_button->name() << " handling mouse release." << std::endl;
#endif
}

void bsTool::handleMouseMove(QMouseEvent *) {
#ifndef NDEBUG
  //std::cerr << m_tool_button->name() << " handling mouse move." << std::endl;
#endif
}

void bsTool::handleMouseDoubleClick(QMouseEvent *) {
#ifndef NDEBUG
  //std::cerr << m_tool_button->name() << " handling mouse double click." << std::endl;
#endif
}

void bsTool::handleMouseEnter(QEvent *) {
#ifndef NDEBUG
  //std::cerr << m_tool_button->name() << " handling mouse enter." << std::endl;
#endif
}

void bsTool::handleKeyPress(QKeyEvent *) {
#ifndef NDEBUG
  //std::cerr << m_tool_button->name() << " handling key press." << std::endl;
#endif
}

void bsTool::handleKeyRelease(QKeyEvent *) {
#ifndef NDEBUG
  //std::cerr << m_tool_button->name() << " handling key release." << std::endl;
#endif
}


void bsTool::paintGL() {
#ifndef NDEBUG
  //std::cerr << m_tool_button->name() << " paint gl." << std::endl;
#endif
}


void bsTool::reset() {
  m_tool_button_act->setChecked(false);
}


void bsTool::setToolButton(bool b) {
    m_tool_button_act->setChecked(b);
    //    m_tool_button_act->setEnabled(b);
}


void bsTool::enableTool(bool b){
    m_tool_button_act->setEnabled(b);
}

void bsTool::updateGL() {
  bsViewer* v = (bsViewer*)m_editor->getViewer();
  v->updateGL();
}

// Utility

sl::point2f bsTool::pixelsToNormalized(QMouseEvent *e) const {
    bsViewer* v = (bsViewer*)m_editor->getViewer();
    return v->pixelsToNormalized(e);
} 


sl::point2f bsTool::worldToNormalized(const sl::point3f& p) const {
    bsViewer* v = (bsViewer*)m_editor->getViewer();
    return v->worldToNormalized(p);
}


sl::point3f bsTool::normalizedToWorldNear(const sl::point2f& p) const {
    bsViewer* v = (bsViewer*)m_editor->getViewer();
    return v->normalizedToWorldNear(p);
}


sl::point3f bsTool::normalizedToWorldFar(const sl::point2f& p) const {
    bsViewer* v = (bsViewer*)m_editor->getViewer();
    return v->normalizedToWorldFar(p);
}    
