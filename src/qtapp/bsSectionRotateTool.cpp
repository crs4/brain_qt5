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
#include "bsSectionRotateTool.h"
#include "bsSectionedBrain.h"
#include "bsSectionViewer2D.h"
#include "bsEditor2D.h"
#include "bsDocument.h"
#include <sl/linear_map_factory.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>
#include <sl/math.hpp>
#include <GL/gl.h>
#include <math.h>
#include <iostream>
//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>

const int STATUS_IDLE = 0;
const int STATUS_DRAG_CENTER = 1;
const int STATUS_ROTATE = 2;

bsSectionRotateTool::bsSectionRotateTool(bsEditor2D* editor,
					 QWidget* parent,
					 const char* name,
					 QPixmap* pixmap,
					 QBitmap* bitmask):
    bsTransformationTool(editor, parent, name, pixmap, bitmask) {
    m_center_radius = 10; // pixels
    m_status = STATUS_IDLE;
}

bsSectionRotateTool::~bsSectionRotateTool() {

}

void bsSectionRotateTool::resetCenter() {
    m_status = STATUS_IDLE;    
    m_center[0] = 0.0f;
    m_center[1] = 0.0f;
    m_center[2] = 0.0f;
    if(m_editor->getDocument())
	if (m_editor->getDocument()->getBrain2D())
	    m_center = m_editor->getDocument()->getBrain2D()->boundingBox().center();
}


void bsSectionRotateTool::activate(bool b) {
    bsTool::activate(b);
    m_status = STATUS_IDLE;    
    updateGL();
}
 

void bsSectionRotateTool::handleMousePress(QMouseEvent *e) {
    if (m_status == STATUS_IDLE) {
      if (e->button() == Qt::LeftButton) {
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    sl::point2f pn_center = worldToNormalized(m_center);
	    if (pn_mouse.distance_squared_to(pn_center) < sl::sqr(2.0f*m_center_radius/(float)m_editor->getViewer()->width())) {
		m_status = STATUS_DRAG_CENTER;
		m_center = normalizedToWorldNear(pn_mouse);
	    } else {
		m_status = STATUS_ROTATE;
		m_start_point = normalizedToWorldNear(pn_mouse);
	    }
	    updateGL();
      } else if (e->button() == Qt::MidButton) {
	    resetCenter();
	}
    }
}

void bsSectionRotateTool::handleMouseRelease(QMouseEvent *) {
    m_status = STATUS_IDLE;
    updateGL();
}

void bsSectionRotateTool::handleMouseMove(QMouseEvent *e) {
    if (m_status == STATUS_DRAG_CENTER) {
	sl::point2f pn_mouse  = pixelsToNormalized(e);
	m_center = normalizedToWorldNear(pn_mouse);
	updateGL();
    } else if (m_status == STATUS_ROTATE) {
	sl::point2f pn_center  = worldToNormalized(m_center);
	sl::point2f pn_start   = worldToNormalized(m_start_point);
	sl::point2f pn_current = pixelsToNormalized(e);

	sl::column_vector2f u = pn_start - pn_center;
	sl::column_vector2f v = pn_current - pn_center;
	float norm = u.two_norm() * v.two_norm();
	float theta = 0.0f;
	if (norm > 1E-6) { 
	    float val = u.dot(v) / norm;
	    if (val<-1.0f) val = -1.0f;
	    if (val> 1.0f) val =  1.0f;
	    theta = acos(val);
	}
	if ((u[0] * v[1] - u[1] * v[0]) < 0) {
	    // clockwise;
	    theta = -theta;
	}

	m_start_point = normalizedToWorldNear(pn_current);

	sl::affine_map2f T0 = sl::linear_map_factory2f::translation(-m_center[0],-m_center[1]);
	sl::affine_map2f T1 = sl::linear_map_factory2f::translation( m_center[0], m_center[1]);
	sl::affine_map2f R  = sl::linear_map_factory2f::rotation(theta);

	transformSection('r', T1*R*T0);

	updateGL();
    }
}

void bsSectionRotateTool::paintGL() {
    glDisable(GL_DEPTH_TEST);
    sl::point2f ch = worldToNormalized(m_center);

    
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
		glVertex2f(ch[0]-rx, ch[1]);
		glVertex2f(ch[0]+rx, ch[1]);
		glVertex2f(ch[0], ch[1]-ry);
		glVertex2f(ch[0], ch[1]+ry);
	    } glEnd();

	    if (m_status == STATUS_ROTATE) {
		sl::point2f sh = worldToNormalized(m_start_point);
		glBegin(GL_LINES); {
		    glVertex2f(ch[0], ch[1]);
		    glVertex2f(sh[0], sh[1]);
		} glEnd();
	    }
	    
	} glPopMatrix();
	
    } glPopMatrix();
}
