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
#ifndef BSSECTIONDEFORMTOOL_H
#define BSSECTIONDEFORMTOOL_H

#include <vector>
#include <cassert>

#include "bsTool2D.h"
#include <sl/fixed_size_point.hpp>
//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>


class bsPointDeformData {
public:
    bsPointDeformData() {
    }

    bsPointDeformData(int p_index, int t_index, const sl::point2f& uv):
	m_point_index(p_index), m_triangle_index(t_index), m_uv(uv) {
    }

    int       m_point_index;
    int       m_triangle_index;
    sl::point2f m_uv;
};

class bsEditor2D;

class bsSectionDeformTool: public bsTool2D {
    Q_OBJECT

public:

    bsSectionDeformTool(bsEditor2D* editor,
			QWidget* parent,
			const char* name,
			QPixmap* pixmap = NULL,
			QBitmap* bitmask = NULL);

    virtual ~bsSectionDeformTool();


public slots: 

    virtual void activate(bool b);

    virtual void handleMousePress(QMouseEvent *e);
    virtual void handleMouseRelease(QMouseEvent *e);
    virtual void handleMouseMove(QMouseEvent *e);
    virtual void handleMouseDoubleClick(QMouseEvent *e);


public:

    virtual void paintGL();

public:

    void init_deform();
    void deform(int deform_vertex, const sl::point3f& p_new);
    void execDeformCmd(const sl::point3f& p);

    inline void activateLowLevel(bool b) {
	bsTool::activate(b);
    }

    inline sl::point3f currentDeformVertexPoint() const {
        assert(m_current_deform_vertex >= 0 &&
	       m_current_deform_vertex < (int)m_current_deformed_vertices.size());
        return m_current_deformed_vertices[m_current_deform_vertex];
    }

    inline sl::point3f currentDeformVertexStartPoint() const {
	return m_deform_vertex_start_point;
    }
    
    inline void setToolStatus(int current_deform_vertex,
			      int status,
			      std::vector<sl::point3f>& vertices,
			      std::vector<sl::tuple3i>& deform_triangulation,
			      std::vector<sl::point3f>& deformed_vertices,
			      std::vector<bsPointDeformData>& deform_section_points,
			      std::vector< std::vector<bsPointDeformData> >& deform_section_neurons){
	 m_current_deform_vertex=current_deform_vertex;
	 m_status=status;
	 m_vertices.clear();
	 m_vertices=vertices;
	 m_current_deform_triangulation.clear();
	 m_current_deform_triangulation=deform_triangulation;
	 m_current_deformed_vertices.clear();
	 m_current_deformed_vertices=deformed_vertices;
	 m_current_deform_section_points.clear();
	 m_current_deform_section_points=deform_section_points;
	 m_current_deform_section_neurons.clear();
	 m_current_deform_section_neurons=deform_section_neurons;
	 updateGL();
    }



protected:

    int getDeformVertex(const sl::point2f& p) const;

protected:
    
    std::vector<sl::point3f> m_vertices; // world

    int       m_status;
    int       m_current_deform_vertex;

    std::vector<sl::point3f>                   m_current_deformed_vertices;
    std::vector<sl::tuple3i>                   m_current_deform_triangulation;
    std::vector<bsPointDeformData>           m_current_deform_section_points;
    std::vector< std::vector<bsPointDeformData> > m_current_deform_section_neurons;
    sl::point3f m_deform_vertex_start_point;

};

#endif
