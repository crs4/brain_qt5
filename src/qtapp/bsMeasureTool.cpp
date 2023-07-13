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
#include "bsMeasureTool.h"
#include "bsMeasureWidgetImpl.h"
#include "bsViewer.h"
#include "bsEditor2D.h"
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>

#include <qstring.h>
//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>

#include <GL/gl.h>
#include <iostream>

const int STATUS_IDLE = 0;
const int STATUS_MEASURING = 1;
const int STATUS_MEASURING_IDLE = 2;

bsMeasureTool::bsMeasureTool(bsEditor2D* editor,
			     QWidget* parent,
			     const char* name,
			     QPixmap* pixmap,
			     QBitmap* bitmask,
			     int cursor_type):
    bsTool2D(editor, parent, name, pixmap, bitmask, cursor_type) {
    m_measure_window = new bsMeasureWidgetImpl;
    m_center_radius = 5; // pixels
    m_status = STATUS_IDLE;
}

bsMeasureTool::~bsMeasureTool() {
}

void bsMeasureTool::activate(bool b) {
    bsTool::activate(b);
    m_status = STATUS_IDLE;    
    bsViewer* v = m_editor->getViewer();
    v->setMouseTracking(b);
    if (b) {
	updateMeasureWindow();
	m_measure_window->show();
    } else {
	m_measure_window->hide();
    }
    updateGL();
}
 

void bsMeasureTool::handleMousePress(QMouseEvent *e) {
    switch (m_status) {
    case STATUS_MEASURING_IDLE: 
    case STATUS_IDLE: 
      if (e->button() == Qt::LeftButton) {
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    m_start_point = normalizedToWorldNear(pn_mouse);
	    m_end_point=m_start_point;
	    updateMeasureWindow();
	    m_status = STATUS_MEASURING;
	}
	break;
    case STATUS_MEASURING: 
      if (e->button() == Qt::LeftButton) {
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    m_end_point = normalizedToWorldNear(pn_mouse);
	    updateMeasureWindow();
	    m_status = STATUS_MEASURING_IDLE;
	}
	break;
    }
}

void bsMeasureTool::handleMouseRelease(QMouseEvent *) {
}

void bsMeasureTool::handleMouseMove(QMouseEvent *e) {
    if (m_status == STATUS_MEASURING) {
	sl::point2f pn_mouse  = pixelsToNormalized(e);
	m_end_point = normalizedToWorldNear(pn_mouse);
	updateMeasureWindow();
	updateGL();
    } 
}

void bsMeasureTool::paintGL() {
    if (m_status != STATUS_IDLE) {
	glDisable(GL_DEPTH_TEST);
	sl::point2f ch_start = worldToNormalized(m_start_point);
	sl::point2f ch_end = worldToNormalized(m_end_point);
	
	
	float rx= 2.0f * m_center_radius / m_editor->getViewer()->width();
	float ry= 2.0f * m_center_radius / m_editor->getViewer()->height();
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); {
	    glLoadIdentity();
	    
	    glMatrixMode(GL_MODELVIEW);
	    glPushMatrix(); {
		glLoadIdentity();
		// DRAW HOMOGENEOUS
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES); {
		    glVertex2f(ch_start[0]-rx, ch_start[1]);
		    glVertex2f(ch_start[0]+rx, ch_start[1]);
		    glVertex2f(ch_start[0], ch_start[1]-ry);
		    glVertex2f(ch_start[0], ch_start[1]+ry);
		} glEnd();
		
		glBegin(GL_LINES); {
		    glVertex2f(ch_start[0], ch_start[1]);
		    glVertex2f(ch_end[0], ch_end[1]);
		} glEnd();
		
		glBegin(GL_LINES); {
		    glVertex2f(ch_end[0]-rx, ch_end[1]);
		    glVertex2f(ch_end[0]+rx, ch_end[1]);
		    glVertex2f(ch_end[0], ch_end[1]-ry);
		    glVertex2f(ch_end[0], ch_end[1]+ry);
		} glEnd();
		
	    } glPopMatrix();
	    
	} glPopMatrix();
    }
}

    
void bsMeasureTool::updateMeasureWindow(){
    if (m_status==STATUS_IDLE) {
	QString str("NULL");
	m_measure_window->setStartPoint(str);
	m_measure_window->setEndPoint(str);
	m_measure_window->setDistance(str);
    } else {
	QString str_start;
	str_start=QString::asprintf("%7.0f %7.0f", m_start_point[0], m_start_point[1]);
	m_measure_window->setStartPoint(str_start);
	QString str_end;
	str_end=QString::asprintf("%7.0f %7.0f", m_end_point[0], m_end_point[1]);
	m_measure_window->setEndPoint(str_end);
	float distance= m_start_point.distance_to(m_end_point);
	QString str_dist;
	str_dist=QString::asprintf("%10.2f", distance);
	m_measure_window->setDistance(str_dist);
    }
}
