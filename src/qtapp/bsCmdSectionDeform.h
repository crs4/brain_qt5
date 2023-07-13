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
#ifndef bsCmdSectionDeform_h
#define bsCmdSectionDeform_h

#include "slCommand.h"
#include <sl/fixed_size_point.hpp>
#include <vector>

const int STATUS_IDLE = 0;
const int STATUS_ADD_VERTEX = 1;
const int STATUS_DEFORM_IDLE = 2;
const int STATUS_DEFORM_VERTEX = 3;
const int STATUS_DEFORM_STOP =4;

class bsSectionDeformTool;
class bsPointDeformData;

class bsCmdSectionDeform: public slUndoableCommand {

public:

    bsCmdSectionDeform(bsSectionDeformTool* tool, int status,
		       int deform_vertex, const sl::point3f& point,
		       std::vector<sl::point3f>& vertices,
		       std::vector<sl::tuple3i>& deform_triangulation,
		       std::vector<sl::point3f>& deformed_vertices,
		       std::vector<bsPointDeformData>& deform_section_points,
		       std::vector< std::vector<bsPointDeformData> >& deform_section_neurons);

    const char *name() const;

    void redo();

    void undo();
 
protected:

    void backup();

protected:


    bsSectionDeformTool* m_tool;
    int  m_status;
    std::vector<sl::point3f> m_vertices;
    int m_current_deform_vertex;
    sl::point3f m_point;
    std::vector<sl::tuple3i> m_deform_triangulation;
    std::vector<sl::point3f> m_deformed_vertices;
    std::vector<bsPointDeformData> m_deform_section_points;
    std::vector< std::vector<bsPointDeformData> > m_deform_section_neurons;

    sl::point3f m_old_point;

}; // bsCmdSectionDeform 

#endif
