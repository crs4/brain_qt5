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
#include "bsSectionEditTool.h"

#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsViewer.h"
#include "bsEditor2D.h"
#include "bsDocument.h"
#include "bsGeometryHelpers.h"
#include "bsCmdSectionEdit.h"

#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>

#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>

#include <GL/gl.h>
#include <iostream>

bsSectionEditTool::bsSectionEditTool(bsEditor2D* editor,
				     QWidget* parent,
				     const char* name,
				     QPixmap* pixmap,
				     QBitmap* bitmask,
				     int cursor_type):
    bsTool2D(editor, parent, name, pixmap, bitmask, cursor_type) {
    m_status = STATUS_IDLE;
    m_current_deform_vertex = -1;
    m_position = 0.0f, 0.0f, 0.0f;
    m_current_section = -1;
    m_viewer=editor->getViewer();
    m_key=-1;
}

bsSectionEditTool::~bsSectionEditTool() {
}


void bsSectionEditTool::activate(bool b) {
    bsTool::activate(b);
    if (b) {
      m_viewer->setFocus();
      m_current_section = editor2D()->getCurrentSectionIndex();
    } else {
      m_viewer->clearFocus();
    }
    m_key=-1;
    updateGL();
}


void bsSectionEditTool::execEdit(int section, int vertex, const sl::point3f position){
  bsDocument* doc = m_editor->getDocument();
  if (doc) {
    bsSectionedBrain* b = doc->getBrain2D();
    if (b->goodSectionIndex(section)) {
      bsSection* s = b->sectionItem(section);
      s->setVertex(vertex,sl::point2f(position[0],position[1]));
    }
    else {
      assert(false);
    }
    doc->emitBrain2DChanged(section);
    updateGL();
  }
}


sl::point3f bsSectionEditTool::currentPosition(int section, int vertex) const{
  bsDocument* doc = m_editor->getDocument();
  sl::point3f p; p = 0.0f, 0.0f, 0.0f;
  if (doc) {
    bsSectionedBrain* b = doc->getBrain2D();
    if (b->goodSectionIndex(section)) {
      bsSection* s = b->sectionItem(section);
      p = s->vertexItem3(vertex);
    }
  }
  return p;
}


const bsSection* bsSectionEditTool::currentSection() {
  m_current_section = editor2D()->getCurrentSectionIndex();
  bsDocument* doc = m_editor->getDocument();
  bsSection* s = NULL;
  if (doc) {
    bsSectionedBrain* b = doc->getBrain2D();
    if (b->goodSectionIndex(m_current_section)) {
      s = b->sectionItem(m_current_section);
    }
  }
  return s;
}



void bsSectionEditTool::handleMousePress(QMouseEvent *e) {
   switch (m_status) {
    case STATUS_IDLE:
      if (e->button() == Qt::LeftButton) {
	  m_current_deform_vertex=-1;
	  sl::point2f pn_mouse  = pixelsToNormalized(e);
	  sl::point3f point = normalizedToWorldNear(pn_mouse);
	  m_current_section = editor2D()->getCurrentSectionIndex();
	  bsDocument* doc = m_editor->getDocument();
	  if (doc) {
	    bsSectionedBrain* b = doc->getBrain2D();
	    if (b->goodSectionIndex(m_current_section)) {
	      bsSection* s = b->sectionItem(m_current_section);
	      float min_value=1e30f;
	      for (int i=0; i<s->vertexCount(); i++) {	
		if (s->vertexItem3(i).distance_squared_to(point) < min_value) {
		  min_value = s->vertexItem3(i).distance_squared_to(point);
		  m_current_deform_vertex=i;
		}
	      }
	      if (m_current_deform_vertex >= 0) {
		m_status = STATUS_EDIT_VERTEX; 
	      }
	    }
	  }
	}
	break;
    default:
      assert(false);
    }
}

void bsSectionEditTool::handleMouseRelease(QMouseEvent *) {
    switch (m_status) {
    case STATUS_IDLE:
	break;
    case STATUS_EDIT_VERTEX: {
        bsDocument* doc = m_editor->getDocument();
	doc->retriangulate(m_current_section);
        m_status = STATUS_IDLE;
	m_current_deform_vertex = -1;
	m_position = 0.0f, 0.0f, 0.0f;	
      }  
      break;
    default:
	assert(false);
    }
    updateGL();
}

void bsSectionEditTool::handleMouseMove(QMouseEvent *e) {
  if (m_status == STATUS_EDIT_VERTEX) {
    sl::point2f pn_mouse  = pixelsToNormalized(e);
    m_position = normalizedToWorldNear(pn_mouse);
    if (m_current_deform_vertex>=0) {
      bsCmdSectionEdit* cmd = new bsCmdSectionEdit(this, m_status, 
						   m_current_section, 
						   m_current_deform_vertex, 
						   m_position);
      m_cmd_processor->doit(cmd);
    }
  }
}


void bsSectionEditTool::handleKeyPress(QKeyEvent *e){
  m_key=e->key();
  updateGL();
}

void bsSectionEditTool::handleKeyRelease(QKeyEvent *){
  m_key=-1;
  updateGL();
}

void bsSectionEditTool::handleMouseEnter(QEvent *){
  m_viewer->setFocus();    
}

void bsSectionEditTool::paintGL() {

  if (m_key == Qt::Key_Shift || m_status==STATUS_EDIT_VERTEX) {
    const bsSection* s = currentSection();
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); {
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix(); {
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(1);
	glPointSize(4);
	glBegin(GL_POINTS); {
	  if (m_status == STATUS_EDIT_VERTEX) {
	    sl::point2f pth = worldToNormalized(m_position);
	    glVertex2f(pth[0],pth[1]);
	  }
	  if (s) {
	    for (int i=0; i<s->vertexCount(); i++) {	
	      sl::point2f pv= worldToNormalized(s->vertexItem3(i));
	      glVertex2f(pv[0],pv[1]);
	    }
	  }
	} glEnd();
	glPointSize(1);
      } glPopMatrix();
    } glPopMatrix();
  }
}

