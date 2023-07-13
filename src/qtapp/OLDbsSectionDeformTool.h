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

#ifdef WIN32
#include <vector>
using namespace std;
#else
#include "vector.h"
#endif

#include "bsTool.h"
#include <sl/fixed_size_point.hpp>

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

class bsSectionDeformTool: public bsTool {
    Q_OBJECT

public:

    bsSectionDeformTool(bsEditorWindow2D* editor,
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

protected:

    int getDeformVertex(const sl::point2f& p) const;

    void init_deform();

    void deform(int i, const sl::point3f& p_new);

protected:
    
    std::vector<sl::point3f> m_vertices; // world

    int       m_status;
    int       m_current_deform_vertex;

    std::vector<sl::point3f>                   m_current_deformed_vertices;
    std::vector<sl::tuple3i>                   m_current_deform_triangulation;
    std::vector<bsPointDeformData>           m_current_deform_section_points;
    std::vector< std::vector<bsPointDeformData> > m_current_deform_section_neurons;
};

#endif
