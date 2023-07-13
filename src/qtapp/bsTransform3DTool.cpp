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
#include "bsTransform3DTool.h"
#include "bsViewer3D.h"
#include "bsEditor3D.h"

#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>
#include <sl/math.hpp>
#include <GL/gl.h>
#include <math.h>
#include <iostream>
//Added by qt3to4:
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>

const int STATUS_IDLE = 0;
const int STATUS_ROTATE = 1;
const int STATUS_TRANSLATE = 2;
const int STATUS_ZOOM = 3;

bsTransform3DTool::bsTransform3DTool(bsEditor3D* editor,
					 QWidget* parent,
					 const char* name,
					 QPixmap* pixmap,
					 QBitmap* bitmask):
    bsTool3D(editor, parent, name, pixmap, bitmask) {
    m_status = STATUS_IDLE;
    m_viewer=editor->getViewer3D();
    m_key=-1;
}


bsTransform3DTool::~bsTransform3DTool() {

}


void bsTransform3DTool::activate(bool b) {
    bsTool::activate(b);
    if (b) {
      m_viewer->setFocus();
    } else {
      m_viewer->clearFocus();
    }
    m_status = STATUS_IDLE; 
    m_key=-1;
    updateGL();
}
 

void bsTransform3DTool::handleMousePress(QMouseEvent *e) {
  if (m_status == STATUS_IDLE) {
    if (e->button() == Qt::LeftButton) {
      sl::point2f pn_mouse  = pixelsToNormalized(e);
      m_start_point = normalizedToWorldNear(pn_mouse);
      switch (m_key) {
      case -1:
	m_status = STATUS_ROTATE;
	break;
      case Qt::Key_Shift:
	m_status = STATUS_ZOOM;
	break;
      case Qt::Key_Control:
	m_status = STATUS_TRANSLATE;
	break;
      }
    } 
  }
}


void bsTransform3DTool::handleMouseRelease(QMouseEvent *e) {
   switch (m_status) {
     case STATUS_ZOOM: {
       sl::point2f pn_here  = pixelsToNormalized(e);
       m_end_point = normalizedToWorldNear(pn_here);
       sl::point2f pn_start = worldToNormalized(m_start_point);
       sl::column_vector2f scale = (pn_here-pn_start)/2.0f;
       sl::point2f pn_c = pn_start + (pn_here-pn_start)/2.0f;
       float sc_value = (fabs(scale[0])+fabs(scale[1]))/2.0f;
       if (sc_value > 1e-06) {
	 sl::affine_map3f T = sl::linear_map_factory3f::translation(-pn_c[0],-pn_c[1],0.0f);
	 sl::affine_map3f S = sl::linear_map_factory3f::scaling(1.0f/sc_value,1.0f/sc_value,1.0f);
	 sl::projective_map3f Pnew = S*T*m_viewer->cameraP();
	 m_viewer->setCameraP(Pnew);
       } else {
	 m_viewer->resetP();
       }
     }
     break;
   }
   m_status = STATUS_IDLE;
   updateGL();
}


void bsTransform3DTool::handleMouseMove(QMouseEvent *e) {
  switch (m_status) {
    case STATUS_ROTATE: {
      sl::point2f pn_here  = pixelsToNormalized(e);
      sl::point2f pn_start = worldToNormalized(m_start_point);
      sl::column_vector2f delta_angle = (pn_here-pn_start);
      
      // Rotation
      sl::point3f trasl = m_viewer->initBox().center();
      sl::affine_map3f T0 = sl::linear_map_factory3f::translation(-trasl[0],-trasl[1],-trasl[2]);
      sl::affine_map3f T1 = sl::linear_map_factory3f::translation( trasl[0], trasl[1], trasl[2]);
      sl::column_vector3f v_s, v_sh, t;
      sl::quaternionf q;
      m_viewer->cameraV().factorize_to(v_s, v_sh, q, t);
      t.to_zero();
      sl::affine_map3f Vr = sl::affine_map3f(v_s, v_sh, q, t);
      sl::affine_map3f delta_R  =
	Vr.inverse() *
	sl::linear_map_factory3f::rotation(1,delta_angle[0]) *
	sl::linear_map_factory3f::rotation(0,-delta_angle[1]) *
	Vr;
      sl::affine_map3f Vnew=m_viewer->cameraV()*T1*delta_R*T0;
      m_viewer->setCameraV(Vnew);
      updateGL();
      m_start_point=normalizedToWorldNear(pn_here);
    }
    break;
    case STATUS_TRANSLATE: {
      sl::point3f trasl = m_viewer->initBox().center();
      sl::affine_map3f T0 = sl::linear_map_factory3f::translation(-trasl[0],-trasl[1],-trasl[2]);
      sl::affine_map3f T1 = sl::linear_map_factory3f::translation( trasl[0], trasl[1], trasl[2]);
      sl::point2f pn_here  = pixelsToNormalized(e);
      sl::point2f pn_start = worldToNormalized(m_start_point);
      sl::column_vector2f pan_vector = (pn_here-pn_start)*m_viewer->initBox().diagonal().two_norm();
      sl::column_vector3f v_s, v_sh, t;
      sl::quaternionf q;
      m_viewer->cameraV().factorize_to(v_s, v_sh, q, t);
      t.to_zero();
      sl::affine_map3f Vr = sl::affine_map3f(v_s, v_sh, q, t);
      sl::affine_map3f delta_V  =
	Vr.inverse() *
	sl::linear_map_factory3f::translation(pan_vector[0],pan_vector[1],0.0f)*
	Vr;
      sl::affine_map3f Vnew=m_viewer->cameraV()*T1*delta_V*T0;
      m_viewer->setCameraV(Vnew);
      updateGL();
      m_start_point=normalizedToWorldNear(pn_here);
      updateGL();
    }
    break;
    case STATUS_ZOOM: {
      sl::point2f pn_mouse  = pixelsToNormalized(e);
      m_end_point = normalizedToWorldNear(pn_mouse);
      updateGL();
    }
    break;
  }    
}

void bsTransform3DTool::handleKeyPress(QKeyEvent *e){
  m_key=e->key();
}

void bsTransform3DTool::handleKeyRelease(QKeyEvent *){
  m_key=-1;
}

void bsTransform3DTool::handleMouseEnter(QEvent *){
      m_viewer->setFocus();
}

void bsTransform3DTool::updateGL() {
  bool b = m_viewer->fancyRenderingEnabled();
  if (m_status != STATUS_IDLE) {
    m_viewer->setFancyRendering(false);
  }
  bsTool::updateGL();
   m_viewer->setFancyRendering(b);
}


void bsTransform3DTool::paintGL() {
  switch (m_status) {
    case STATUS_ZOOM: {
      glDisable(GL_DEPTH_TEST);
      sl::point2f ch_start = worldToNormalized(m_start_point);
      sl::point2f ch_end = worldToNormalized(m_end_point);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix(); {
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
	  glLoadIdentity();
	  // DRAW HOMOGENEOUS
	  glColor3f(1.0, 0.0, 0.0);
	  glBegin(GL_LINE_LOOP); {
	    glVertex2f(ch_start[0], ch_start[1]);
	    glVertex2f(ch_end[0], ch_start[1]);
	    glVertex2f(ch_end[0], ch_end[1]);
	    glVertex2f(ch_start[0], ch_end[1]);
	  } glEnd();
	} glPopMatrix();
	
      } glPopMatrix();
    }
    break;
  }
}
