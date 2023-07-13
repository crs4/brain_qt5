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
#include "bsCmdPanZoom.h"
#include "bsViewer2D.h"

bsCmdPanZoom::bsCmdPanZoom(bsViewer2D* v,
			   const char motion_type,
			   int motion_value) {
  m_viewer = v;
  m_type = motion_type;
  m_value = motion_value;
  m_old_zoom_value = -1;
}

const char* bsCmdPanZoom::name() const {
  switch (m_type) {
  case 'x': return "Pan X"; break;
  case 'y': return "Pan Y"; break;
  case 'z': return "Zoom"; break;
  case 'r': return "Reset"; break;
  }
  return NULL;
}  

void bsCmdPanZoom::redo() {
  switch (m_type) {
  case 'x': m_viewer->setXTranslationCmd(m_value); break;
  case 'y': m_viewer->setYTranslationCmd(m_value); break;
  case 'z': m_viewer->setZoomFactorCmd(m_value); break;
  case 'r': m_viewer->resetCmd(); break;
  }
}

void bsCmdPanZoom::undo() {
  switch (m_type) {
  case 'x': 
    m_viewer->setXTranslationCmd(m_old_x_value); 
    break;
  case 'y': 
    m_viewer->setYTranslationCmd(m_old_y_value); 
    break;
  case 'z': 
    m_viewer->setZoomFactorCmd(m_old_zoom_value);
    break;
  case 'r':
    m_viewer->setXTranslationCmd(m_old_x_value); 
    m_viewer->setYTranslationCmd(m_old_y_value); 
    m_viewer->setZoomFactorCmd(m_old_zoom_value);
    break;
  }
}

void bsCmdPanZoom::backup() {
  m_old_zoom_value = m_viewer->getZoomFactor();
  m_old_x_value    = m_viewer->getXTranslation();
  m_old_y_value    = m_viewer->getYTranslation();
}

bool bsCmdPanZoom::isComposable( slCommand *c ) const {
  assert(c);
  
  bool result = false;
  bsCmdPanZoom* c_new = dynamic_cast<bsCmdPanZoom*>(c);
  if (c_new) {
    result = m_type == c_new->m_type; 
  }

  return result;
}

void bsCmdPanZoom::composeAndDestroy( slCommand *c) {
  assert(c);
  assert(!c->isLinked());
  assert(isComposable(c));

  bsCmdPanZoom* c_new = dynamic_cast<bsCmdPanZoom*>(c);
  
  m_value = c_new->m_value;

  delete c;
}

