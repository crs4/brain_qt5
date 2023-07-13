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
#include "bsSectionDeformTool.h"

#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsViewer.h"
#include "bsEditor2D.h"
#include "bsDocument.h"
#include "bsGeometryHelpers.h"
#include "bsCmdSectionDeform.h"

#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>

#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>

#include <GL/gl.h>
#include <iostream>

bsSectionDeformTool::bsSectionDeformTool(bsEditor2D* editor,
					 QWidget* parent,
					 const char* name,
					 QPixmap* pixmap,
					 QBitmap* bitmask):
    bsTool2D(editor, parent, name, pixmap, bitmask) {
    m_status = STATUS_IDLE;
    m_current_deform_vertex = -1;
}

bsSectionDeformTool::~bsSectionDeformTool() {
}

void bsSectionDeformTool::execDeformCmd(const sl::point3f& p){
	bsCmdSectionDeform* cmd = new bsCmdSectionDeform(this, m_status,
							 m_current_deform_vertex, p, 
							 m_vertices,
							 m_current_deform_triangulation,
							 m_current_deformed_vertices,
							 m_current_deform_section_points,
							 m_current_deform_section_neurons);
	m_cmd_processor->doit(cmd);
  
}

void bsSectionDeformTool::activate(bool b) {
    bsTool::activate(b);
    if (!b) {
	if (m_status==STATUS_DEFORM_IDLE) {
	    m_status = STATUS_DEFORM_STOP;
	    execDeformCmd(sl::point3f(0.0,0.0,0.0));
	}
    } else {
	m_status=STATUS_IDLE;
	m_current_deform_vertex = -1;
	m_vertices.clear();
	m_current_deformed_vertices.clear();
	m_current_deform_triangulation.clear();
	m_current_deform_section_points.clear();
	m_current_deform_section_neurons.clear();
    }

    bsViewer* v = m_editor->getViewer();
    v->setMouseTracking(b);
    updateGL();
}
 

int bsSectionDeformTool::getDeformVertex(const sl::point2f& p) const {
    int result = -1;
    double d_min = 1e30;
    int i_min = -1;
    for (int i=0; i< (int)m_current_deformed_vertices.size(); i++) {
	sl::point2f p_i = worldToNormalized(m_current_deformed_vertices[i]);
	double d = p.distance_squared_to(p_i);
	if (d<d_min) {
	    d_min = d;
	    i_min = i;
	}
    }
    const double click_radius = 5;
    if (i_min >= 0 && (d_min < sl::sqr(2.0*click_radius/(float)m_editor->getViewer()->width()))) {
	result = i_min;
    }
    return result;
}

void bsSectionDeformTool::handleMousePress(QMouseEvent *e) {
    switch (m_status) {
    case STATUS_IDLE:
      if (e->button() == Qt::LeftButton) {
	    m_status = STATUS_ADD_VERTEX;
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    sl::point3f point = normalizedToWorldNear(pn_mouse);
	    m_vertices.push_back(point);
	}
	break;
    case STATUS_ADD_VERTEX:
      if (e->button() == Qt::LeftButton) {
	    m_status = STATUS_ADD_VERTEX;
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    sl::point3f point = normalizedToWorldNear(pn_mouse);
	    bool accept = true;
	    for (unsigned int i=0; (i<m_vertices.size()-1) && (accept); i++) {
	      accept = (m_vertices[i].distance_squared_to(point) > 0.0f);
	      std::cerr << i << " - DIST " << m_vertices[i].distance_squared_to(point) << " " << accept << std::endl;
	    }
	    if (accept) {
	      m_vertices.push_back(point);
	    }
	    updateGL();
	}
	break;
    case STATUS_DEFORM_STOP:
    case STATUS_DEFORM_IDLE:
      if (e->button() == Qt::LeftButton) {
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    m_current_deform_vertex = getDeformVertex(pn_mouse);
	    if (m_current_deform_vertex >= 0) {
		m_status = STATUS_DEFORM_VERTEX;
		m_deform_vertex_start_point = currentDeformVertexPoint();
		deform(m_current_deform_vertex, normalizedToWorldNear(pn_mouse));
		updateGL();
	    }
	}
	break;
    case STATUS_DEFORM_VERTEX:
	break;
    default:
	assert(false);
    }
}

void bsSectionDeformTool::handleMouseRelease(QMouseEvent *e) {
    switch (m_status) {
    case STATUS_IDLE:
	break;
    case STATUS_ADD_VERTEX:
	if (m_vertices.size() == 1) {
	  m_vertices.push_back(m_vertices[0]);
	  updateGL();
	}  
	break;
    case STATUS_DEFORM_IDLE:
	break;
    case STATUS_DEFORM_VERTEX:
	{
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    sl::point3f p = normalizedToWorldNear(pn_mouse);
	    execDeformCmd(p);
	    int current_section = editor2D()->getCurrentSectionIndex();
	    bsDocument* doc = m_editor->getDocument();
	    doc->retriangulate(current_section);
	    updateGL();
		
	    m_status =  STATUS_DEFORM_IDLE;
	    m_current_deform_vertex = -1;
	}
	break;
    default:
	assert(false);
    }
}

void bsSectionDeformTool::handleMouseDoubleClick(QMouseEvent *e) {
    switch (m_status) {
    case STATUS_IDLE:
	break;
    case STATUS_ADD_VERTEX:
      if (e->button() == Qt::LeftButton) {
	    m_vertices.pop_back();
	    if (m_vertices.size() >= 3) {
		m_status = STATUS_DEFORM_IDLE;
		m_current_deform_vertex = -1;
		init_deform();
		assert(m_vertices.size() == m_current_deformed_vertices.size());
		execDeformCmd(sl::point3f(0.0,0.0,0.0));
	    } else {
		m_vertices.clear();
		m_current_deformed_vertices.clear();
		m_current_deform_triangulation.clear();
		m_current_deform_section_points.clear();
		m_current_deform_section_neurons.clear();
		m_status = STATUS_IDLE;
		updateGL();
	    }
	}
	break;
    case STATUS_DEFORM_IDLE:
	break;
    case STATUS_DEFORM_VERTEX:
	break;
    case STATUS_DEFORM_STOP:
	break;
    default:
	assert(false);
    }
}

void bsSectionDeformTool::handleMouseMove(QMouseEvent *e) {
    switch (m_status) {
    case STATUS_IDLE:
	break;
    case STATUS_ADD_VERTEX:
	{
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    sl::point3f point = normalizedToWorldNear(pn_mouse);
	    m_vertices[m_vertices.size()-1]=point;
	    updateGL();
	}
	break;
    case STATUS_DEFORM_STOP:
	break;
    case STATUS_DEFORM_IDLE:
	break;
    case STATUS_DEFORM_VERTEX:
	{
	    assert(m_current_deform_vertex >= 0 && m_current_deform_vertex < (int) m_current_deformed_vertices.size());
	    sl::point2f pn_mouse  = pixelsToNormalized(e);
	    deform(m_current_deform_vertex, normalizedToWorldNear(pn_mouse));
	    updateGL();
	}
	break;
    default:
	assert(false);
    }
}


void bsSectionDeformTool::init_deform() {
#ifndef NDEBUG
    std::cerr << "INIT DEFORM!" << std::endl;
#endif
    m_current_deformed_vertices = m_vertices;

    std::vector<sl::point2f> tmpvertices(m_vertices.size());
    std::vector<int>       tmpconnectivity(m_vertices.size());
    { 
	for (int i=0; i< (int)m_vertices.size(); i++) {
	    tmpvertices[i] = sl::point2f( m_vertices[i][0], m_vertices[i][1] );
	    tmpconnectivity[i] = i;
	}
    }

    m_current_deform_triangulation = bsGeometryHelpers::triangulation(tmpconnectivity,
								      tmpvertices);
    {
	int current_section = editor2D()->getCurrentSectionIndex();
	bsDocument* doc = m_editor->getDocument();
	if (doc) {
	    bsSectionedBrain* b = doc->getBrain2D();
	    if (b->goodSectionIndex(current_section)) {
		bsSection* s = b->sectionItem(current_section);
		{
		    m_current_deform_section_points.clear();
		    for (int i=0; i<s->vertexCount(); i++) {
			std::pair<int, sl::point2f> tc = bsGeometryHelpers::triangleCoordinates(m_current_deform_triangulation,
											 tmpvertices,
											 s->vertexItem(i));
			if (tc.first >= 0) {
			    m_current_deform_section_points.push_back(bsPointDeformData(i, tc.first, tc.second));
			}
		    }
		}

		// NEURONS!!!
		{
		    m_current_deform_section_neurons.clear();
		    for (int tp=0; tp<=s->maxNeuronType(); tp++) {
			std::vector<bsPointDeformData> empty;
			m_current_deform_section_neurons.push_back(empty);
			for (int i=0; i<s->neuronCount(tp); i++) {
			    std::pair<int, sl::point2f> tc = 
				bsGeometryHelpers::triangleCoordinates(m_current_deform_triangulation,
								       tmpvertices,
								       s->neuronItem(tp,i));
			    if (tc.first >= 0) {
				m_current_deform_section_neurons[tp].push_back(bsPointDeformData(i, tc.first, tc.second));
			    }
			}
		    }
		}
		updateGL();
	    }
	}
    }
}

void bsSectionDeformTool::deform(int deform_vertex, const sl::point3f& p_new) {
    assert(deform_vertex >= 0 && deform_vertex < (int)m_current_deformed_vertices.size());
    m_current_deformed_vertices[deform_vertex] = p_new;

    {
	int current_section = editor2D()->getCurrentSectionIndex();
	bsDocument* doc = m_editor->getDocument();
	if (doc) {
	    bsSectionedBrain* b = doc->getBrain2D();
	    if (b->goodSectionIndex(current_section)) {
		bsSection* s = b->sectionItem(current_section);
		{
		    for (int i=0; i<(int)m_current_deform_section_points.size(); i++) {
			bsPointDeformData data = m_current_deform_section_points[i];
			sl::tuple3i tri = m_current_deform_triangulation[data.m_triangle_index];
			sl::point2f P1 = sl::point2f(m_current_deformed_vertices[tri[0]][0], 
						 m_current_deformed_vertices[tri[0]][1]);
			sl::point2f P2 = sl::point2f(m_current_deformed_vertices[tri[1]][0], 
						 m_current_deformed_vertices[tri[1]][1]);
			sl::point2f P3 = sl::point2f(m_current_deformed_vertices[tri[2]][0], 
						 m_current_deformed_vertices[tri[2]][1]);
			
			sl::point2f deformed_point = bsGeometryHelpers::baryToCart(data.m_uv, 
										 P1,
										 P2,
										 P3);
			
			s->setVertex(data.m_point_index, deformed_point);
		    }
		}

		// NEURONS!!!
		{
		    for (int tp=0; tp<(int)m_current_deform_section_neurons.size(); tp++) {
			for (int i=0; i<(int)m_current_deform_section_neurons[tp].size(); i++) {
			    bsPointDeformData data = m_current_deform_section_neurons[tp][i];
			    sl::tuple3i tri = m_current_deform_triangulation[data.m_triangle_index];
			    sl::point2f P1 = sl::point2f(m_current_deformed_vertices[tri[0]][0], 
						     m_current_deformed_vertices[tri[0]][1]);
			    sl::point2f P2 = sl::point2f(m_current_deformed_vertices[tri[1]][0], 
						     m_current_deformed_vertices[tri[1]][1]);
			    sl::point2f P3 = sl::point2f(m_current_deformed_vertices[tri[2]][0], 
						     m_current_deformed_vertices[tri[2]][1]);
			    
			    sl::point2f deformed_neuron = bsGeometryHelpers::baryToCart(data.m_uv, 
										     P1,
										     P2,
										     P3);
			    
			    s->setNeuron(tp,data.m_point_index, deformed_neuron);
			}
		    }
		}
		// Signal: brain2D is changed!!
		doc->emitBrain2DChanged(current_section);
	    }
	}
    }
}


void bsSectionDeformTool::paintGL() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); {
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
	    glLoadIdentity();
	    // DRAW HOMOGENEOUS
	    if (m_current_deformed_vertices.size() > 0) {
		glColor3f(0.5, 0.0, 0.0);
	    } else {
		glColor3f(1.0, 0.0, 0.0);
	    }
	    glLineWidth(1);
	    glBegin(GL_LINE_LOOP); {
		for (int i=0; i<(int)m_vertices.size(); i++) {
		    sl::point2f pth = worldToNormalized(m_vertices[i]);
		    glVertex2f(pth[0],pth[1]);
		}
	    } glEnd();
	    if (m_current_deformed_vertices.size() > 0) {
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_LOOP); {
		    for (int i=0; i<(int)m_current_deformed_vertices.size(); i++) {
			sl::point2f pth = worldToNormalized(m_current_deformed_vertices[i]);
			glVertex2f(pth[0],pth[1]);
		    }
		} glEnd();
		glPointSize(3);
		glBegin(GL_POINTS); {
		    for (int i=0; i<(int)m_current_deformed_vertices.size(); i++) {
			sl::point2f pth = worldToNormalized(m_current_deformed_vertices[i]);
			glVertex2f(pth[0],pth[1]);
		    }
		} glEnd();
		glPointSize(1);
	    }
	} glPopMatrix();
	
    } glPopMatrix();
}

