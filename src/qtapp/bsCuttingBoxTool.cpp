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
#include "bsCuttingBoxTool.h"
#include "bsBrainViewer.h"
#include "bsEditor3D.h"
#include "bsCuttingBoxWidgetImpl.h"

#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>
#include <sl/math.hpp>
#include <sl/axis_aligned_box.hpp>
#include <sl/fixed_size_square_matrix.hpp>
#include <GL/gl.h>
#include <math.h>
#include <iostream>
//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>

const int STATUS_IDLE = 0;
const int STATUS_ROTATE = 1;
const int STATUS_TRANSLATE = 2;

bsCuttingBoxTool::bsCuttingBoxTool(bsEditor3D* editor,
					 QWidget* parent,
					 const char* name,
					 QPixmap* pixmap,
					 QBitmap* bitmask):
    bsTool3D(editor, parent, name, pixmap, bitmask) {
    m_cutting_box_window = new bsCuttingBoxWidgetImpl(0);
    m_status = STATUS_IDLE;
    m_viewer=editor->getBrainViewer();
    m_key=-1;

    connect(m_cutting_box_window, SIGNAL(wlhChanged()), this, SLOT(changeLengths()));
    connect(m_cutting_box_window, SIGNAL(resetClicked()), this, SLOT(resetCuttingBox()));
    connect(m_viewer, SIGNAL(newBrain2D(bool)), this, SLOT(resetCuttingBox()));

    connect(this, SIGNAL(emitArbitrarySlicing(bool)), m_cutting_box_window, SLOT(enableSliders(bool)));
    connect(m_cutting_box_window, SIGNAL(slidersChanged()), this, SLOT(traslateIJKCuttingBox()));
}


bsCuttingBoxTool::~bsCuttingBoxTool() {

}


void bsCuttingBoxTool::activate(bool b) {
    bsTool::activate(b);
    m_viewer->setCuttingToolEnabled(b);
    if (b) {
      m_viewer->setFocus();
      updateCuttingBoxWindow();
      m_cutting_box_window->show();
      m_viewer->setFancyRendering(false);
    } else {
      m_viewer->clearFocus();
      m_cutting_box_window->hide();
      m_viewer->setFancyRendering(true);
    }
    m_status = STATUS_IDLE; 
    m_key=-1;
    updateGL();
}
 

void bsCuttingBoxTool::handleMousePress(QMouseEvent *e) {
  if (m_status == STATUS_IDLE) {
    if (e->button() == Qt::LeftButton) {
      emit emitArbitrarySlicing(false);
      sl::point2f pn_mouse  = pixelsToNormalized(e);
      m_start_point = normalizedToWorldNear(pn_mouse);
      switch (m_key) {
      case -1:
	m_status = STATUS_ROTATE;
	break;
      case Qt::Key_Control:
	m_status = STATUS_TRANSLATE;
	break;
      }
    } 
  }
}


void bsCuttingBoxTool::handleMouseRelease(QMouseEvent * /*e*/) {
   m_status = STATUS_IDLE;
   updateGL();
}


void bsCuttingBoxTool::handleMouseMove(QMouseEvent *e) {
  switch (m_status) {
    case STATUS_ROTATE: {
      sl::point2f pn_here  = pixelsToNormalized(e);
      sl::point2f pn_start = worldToNormalized(m_start_point);
      sl::column_vector2f delta_angle = (pn_here-pn_start);
      
      sl::point3f trasl = m_viewer->getCuttingBox().center();
      sl::rigid_body_map3f T0 = sl::linear_map_factory3f::translation(-trasl[0],-trasl[1],-trasl[2]);
      sl::rigid_body_map3f T1 = sl::linear_map_factory3f::translation( trasl[0], trasl[1], trasl[2]);
      sl::column_vector3f v_s, v_sh, t;
      sl::quaternionf q;
      m_viewer->cameraV().factorize_to(v_s, v_sh, q, t);
      t.to_zero();
      sl::rigid_body_map3f Vr = sl::linear_map_factory3f::rotation(q);
      sl::rigid_body_map3f delta_R  =
	Vr.inverse() *
	sl::linear_map_factory3f::rotation(1,delta_angle[0]) *
	sl::linear_map_factory3f::rotation(0,-delta_angle[1]) *
	Vr;
      sl::rigid_body_map3f Mnew=T1*delta_R*T0*m_viewer->getCuttingBox().from_box_space_map();
      m_viewer->setCuttingBox(sl::obox3f(Mnew,m_viewer->getCuttingBox().half_side_lengths()));
      updateGL();
      m_start_point=normalizedToWorldNear(pn_here);
      emit cuttingBoxChanged();
    }
    break;
    case STATUS_TRANSLATE: {
      sl::point3f trasl = m_viewer->getCuttingBox().center();
      sl::rigid_body_map3f T0 = sl::linear_map_factory3f::translation(-trasl[0],-trasl[1],-trasl[2]);
      sl::rigid_body_map3f T1 = sl::linear_map_factory3f::translation( trasl[0], trasl[1], trasl[2]);
      sl::point2f pn_here  = pixelsToNormalized(e);
      sl::point2f pn_start = worldToNormalized(m_start_point);
      sl::column_vector2f pan_vector = (pn_here-pn_start)*m_viewer->getCuttingBox().diagonal().two_norm();
      sl::column_vector3f v_s, v_sh, t;
      sl::quaternionf q;
      m_viewer->cameraV().factorize_to(v_s, v_sh, q, t);
      t.to_zero();

      sl::rigid_body_map3f Vr = sl::linear_map_factory3f::rotation(q);
      sl::rigid_body_map3f delta_R  =
	Vr.inverse() *
	sl::linear_map_factory3f::translation(pan_vector[0],pan_vector[1],0.0f)*
	Vr;
      sl::rigid_body_map3f Mnew=T1*delta_R*T0*m_viewer->getCuttingBox().from_box_space_map();
      m_viewer->setCuttingBox(sl::obox3f(Mnew,m_viewer->getCuttingBox().half_side_lengths()));

      updateGL();
      m_start_point=normalizedToWorldNear(pn_here);
      emit cuttingBoxChanged();
    }
    break;
  }    
}

void bsCuttingBoxTool::handleKeyPress(QKeyEvent *e){
  m_key=e->key();
}

void bsCuttingBoxTool::handleKeyRelease(QKeyEvent *){
  m_key=-1;
}

void bsCuttingBoxTool::handleMouseEnter(QEvent *){
      m_viewer->setFocus();
}


void bsCuttingBoxTool::updateCuttingBoxWindow() {
  sl::obox3f cb = m_viewer->getCuttingBox();
  sl::vector3f v = cb.half_side_lengths();

  m_cutting_box_window->setW(v[0]*2);
  m_cutting_box_window->setL(v[1]*2);
  m_cutting_box_window->setH(v[2]*2);

  sl::obox3f bb = m_viewer->getBoundingBox();
  sl::point3f center_bb = bb.center();
  sl::vector3f vo = bb.half_side_lengths();

  sl::point3f center = cb.center();
  sl::matrix4f m = cb.from_box_space_map().as_matrix();
  
  if( m(0,0)==1.0f && m(1,1)==1.0f && m(2,2)==1.0f ) {
    m_cutting_box_window->enableSliders(true);
    sl::vector3f value;
    value = (center[0]-center_bb[0]),(center[1]-center_bb[1]),(center[2]-center_bb[2]);
    m_cutting_box_window->setSliders(value[0]/vo[0],value[1]/vo[1],value[2]/vo[2]);
  } else {
    m_cutting_box_window->enableSliders(false);
    m_cutting_box_window->setSliders(0.0f,0.0f,0.0f);
  }
}


void bsCuttingBoxTool::changeLengths() {
  sl::vector3f v; 
  v[0] = m_cutting_box_window->getW()/2.0f;
  v[1] = m_cutting_box_window->getL()/2.0f;
  v[2] = m_cutting_box_window->getH()/2.0f;

  sl::obox3f cb = sl::obox3f(m_viewer->getCuttingBox().from_box_space_map(),v);
  if (cb.volume() > 0.0f) {
    m_viewer->setCuttingBox(cb);
    updateGL();
  }
  emit cuttingBoxChanged();
}

void bsCuttingBoxTool::resetCuttingBox() {
  m_viewer->resetCuttingBox();
  updateCuttingBoxWindow();
  updateGL();
  emit cuttingBoxChanged();
}

void bsCuttingBoxTool::traslateIJKCuttingBox() {
  sl::obox3f ob=m_viewer->getCuttingBox();
  sl::point3f trasl_ijk_value;
  sl::obox3f bb = m_viewer->getBoundingBox();
  sl::vector3f bb_half_lengths = bb.half_side_lengths();
  trasl_ijk_value = m_cutting_box_window->normalizeSlider(0)*bb_half_lengths[0],
                    m_cutting_box_window->normalizeSlider(1)*bb_half_lengths[1],
                    m_cutting_box_window->normalizeSlider(2)*bb_half_lengths[2];
 
  sl::rigid_body_map3f T = 
    sl::linear_map_factory3f::translation(trasl_ijk_value[0],
					  trasl_ijk_value[1],
					  trasl_ijk_value[2]);
  sl::rigid_body_map3f Mnew=bb.from_box_space_map()*T;
  m_viewer->setCuttingBox(sl::obox3f(Mnew,ob.half_side_lengths()));
  updateGL();
  emit cuttingBoxChanged();
}

