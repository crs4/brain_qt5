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
#include "bsCmdSectionDeform.h"
#include "bsSectionDeformTool.h"

#include <qtoolbutton.h>

bsCmdSectionDeform::bsCmdSectionDeform(bsSectionDeformTool* tool, int status,
				       int deform_vertex, const sl::point3f& point,
				       std::vector<sl::point3f>& vertices,
				       std::vector<sl::tuple3i>& deform_triangulation,
				       std::vector<sl::point3f>& deformed_vertices,
				       std::vector<bsPointDeformData>& deform_section_points,
				       std::vector< std::vector<bsPointDeformData> >& deform_section_neurons) {
  m_tool = tool;
  m_status = status;
  m_current_deform_vertex = deform_vertex;
  m_point = point;
  m_vertices=vertices;
  m_deform_triangulation=deform_triangulation;
  m_deformed_vertices=deformed_vertices;
  m_deform_section_points=deform_section_points;
  m_deform_section_neurons=deform_section_neurons;
}

const char* bsCmdSectionDeform::name() const {
    switch (m_status) {
    case STATUS_DEFORM_IDLE: return "Section Deform: Init"; break;
    case STATUS_DEFORM_VERTEX: return "Section Deform: Run"; break;
    case STATUS_DEFORM_STOP: return "Section Deform: Stop"; break;
    }
    return NULL;
}  


void bsCmdSectionDeform::redo() {
    switch (m_status) {
    case STATUS_DEFORM_IDLE:
	if (!m_tool->isActive()) {
	    m_tool->setToolButton(true);
	}
	m_tool->setToolStatus(m_current_deform_vertex,
			      m_status,
			      m_vertices,
			      m_deform_triangulation,
			      m_deformed_vertices,
			      m_deform_section_points,
			      m_deform_section_neurons);
	
	break;
    case STATUS_DEFORM_VERTEX:
	m_tool->deform(m_current_deform_vertex, m_point);
	break;
    case STATUS_DEFORM_STOP:
	if (m_tool->isActive()) {
	    m_tool->setToolButton(false);
	}
	break;
    }
}

void bsCmdSectionDeform::undo() {
    switch (m_status) {
    case STATUS_DEFORM_IDLE:
	m_tool->setToolButton(false);
	break;
    case STATUS_DEFORM_VERTEX:
	if (m_current_deform_vertex >=0) {
	    m_tool->deform(m_current_deform_vertex, m_old_point);
	}
	break;
    case STATUS_DEFORM_STOP:
	if (!m_tool->isActive()) { 
	    m_tool->setToolButton(true);
	}
	m_tool->setToolStatus(m_current_deform_vertex,
			      m_status,
			      m_vertices,
			      m_deform_triangulation,
			      m_deformed_vertices,
			      m_deform_section_points,
			      m_deform_section_neurons);
	break;
    }
}

void bsCmdSectionDeform::backup() {
    m_old_point=m_tool->currentDeformVertexStartPoint();
}

