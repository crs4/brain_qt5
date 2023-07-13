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
// .NAME bsSection


#ifndef _BS_SECTION_H_
#define _BS_SECTION_H_


#include <sl/affine_map.hpp>
#include <sl/fixed_size_point.hpp>
#include <sl/projective_map.hpp>
#include <sl/axis_aligned_box.hpp>
#include <vector>
#include <cassert>

class bsCortexRegion;
class bsSubRegion;


class bsSection
{

public: // Creation & Destruction

  bsSection();

  bsSection(const std::vector< sl::point2f >&   points,
	    std::vector< int >                external_boundary_connectivity,
	    std::vector< int >                internal_boundary_connectivity,
	    std::vector< std::vector<int> >        holes_or_boundaries,
	    std::vector< sl::tuple2i >          cuts,
	    std::vector< int >                region_ids);

  bsSection(const std::vector< sl::point2f >&   points,
	    const std::vector< std::vector<sl::point2f> >&   allneurons,
	    const float                         quota);

  virtual ~bsSection();

public: // Queries -- quota and thickness
  
  inline float getQuota() const { 
    return m_quota; 
  }

  inline float getUpThickness() const { 
    return m_up_thickness; 
  }

  inline float getDownThickness() const { 
    return m_down_thickness; 
  }

  inline float getThickness() const {
    return (getUpThickness()+getDownThickness());
  }

  inline float getNeuronUpThickness() const { 
    return m_neuron_up_thickness; 
  }

  inline float getNeuronDownThickness() const { 
    return m_neuron_down_thickness; 
  }

  inline float getNeuronThickness() const {
    return (getNeuronUpThickness()+getNeuronDownThickness());
  }

public: // Queries -- vertices

  inline const std::vector<sl::point2f>& vertices() const {
      return m_allpoints;
  }

  inline int vertexCount() const { 
    return m_allpoints.size(); 
  }

  inline bool goodVertexIndex(int i) const {
    return i>=0 && i<vertexCount();
  }

  inline sl::point2f vertexItem(int i) const {
    assert(goodVertexIndex(i));
    return m_allpoints[i];
  }

  inline sl::point3f vertexItem3(int i) const {
    assert(goodVertexIndex(i));
    sl::point2f p2 = vertexItem(i);
    return sl::point3f(p2[0], p2[1], getQuota());
  }

public: // Queries -- Box

  inline const sl::aabox3f& boundingBox() const {
    if (!m_bounding_box_up_to_date) {
      ((bsSection*)this)->computeBoundingBox();
    }
    assert(m_bounding_box_up_to_date);
    return m_bounding_box;
  }

public: // Queries -- Intersection
  inline bool regularSection() const {
    return m_regular_section;
  }

public: // Queries -- Polygons

  bool isCCW(const std::vector<int>& c) const;

  float signedArea(const std::vector<int>& c) const;
  
  bool isInside(const std::vector<int>& c,
		const sl::point2f& P) const;

  bool  isInside(const std::vector<int>& c1,
		 const std::vector<int>& c2) const;
 

public: // Queries -- Geometry


public: // Queries -- neurons

  inline int neuronCount(int tp) const { 
    int result = 0;
    if (tp>=0 && tp < (int)m_allneurons.size()) {
      result = m_allneurons[tp].size();
    }
    return result;
  }

  int neuronCount() const;

  inline int maxNeuronType() const {
    return m_allneurons.size()-1;
  }

  inline bool goodNeuronIndex(int tp, int i) const {
    return i>=0 && i<neuronCount(tp);
  }

  inline sl::point2f neuronItem(int tp, int i) const {
    assert(goodNeuronIndex(tp,i));
    return m_allneurons[tp][i];
  }

  inline sl::point3f neuronItem3(int tp, int i) const {
    assert(goodNeuronIndex(tp, i));
    sl::point2f p2 = neuronItem(tp,i);
    return sl::point3f(p2[0], p2[1], getQuota());
  }

public: // Queries - Cortex region

  inline int cortexRegionCount() const {
    return m_cortex_regions.size();
  }

  inline bool goodCortexRegionIndex(int i) const {
    return i>= 0 && i< cortexRegionCount();
  }

  inline const bsCortexRegion* cortexRegionItem(int i) const {
    assert(goodCortexRegionIndex(i));
    return m_cortex_regions[i];
  }

  inline bsCortexRegion* cortexRegionItem(int i) {
    assert(goodCortexRegionIndex(i));
    return m_cortex_regions[i];
  }

  int subRegionCount(int i) const;

  int maxSubRegionId() const;


public: // Queries -- triangles

  inline int triangleCount() const {
    return m_triangles.size();
  }

  inline bool hasTriangulation() const {
    return triangleCount()>0;
  }

  inline bool goodTriangleIndex(int i) const {
    return i>=0 && i< triangleCount();
  }

  inline sl::tuple3i triangleItem(int i) const {
    assert(goodTriangleIndex(i));
    return m_triangles[i];
  }

  inline sl::point2f triangleVertexItem(int i, int v) const {
    assert(goodTriangleIndex(i));
    assert(v>=0 && v<3);
    return vertexItem(m_triangles[i][v]);
  }


  inline sl::point3f triangleVertexItem3(int i, int v) const {
    assert(goodTriangleIndex(i));
    assert(v>=0 && v<3);
    return vertexItem3(m_triangles[i][v]);
  }

  std::vector<sl::tuple3i> getTriangles() const {
    return m_triangles;
  }

public: // Queries -- areas and volumes

  float triangleArea(int i) const;

  float sectionArea() const;
  float sectionVolume() const;

  float cortexRegionArea(int c) const;
  float cortexRegionVolume(int c) const;

  float subRegionItemArea(int c, int s) const;
  float subRegionIdArea(int c, int id) const;
  float subRegionItemVolume(int c, int s) const;
  float subRegionIdVolume(int c, int id) const;

public: // Commands

  void clear(); 

public: //  Command Cortex Region

  void appendCortexRegion(std::vector< int >& external_boundary,
			  int internal_boundary,
			  std::vector< std::vector< int > >& hole,
			  std::vector< int >& id,
			  std::vector< std::vector< int > >& external_contour,
			  std::vector< std::vector< int > >& internal_contour,
			  std::vector< std::vector< int > >& border1,
			  std::vector< std::vector< int > >& border2,
			  std::vector< std::vector< std::vector< int > > >& hole_sub);


public: // Commands -- quota and thickness

  inline void setQuota(float quota) {
    m_quota = quota;
  }

  inline void setUpThickness(float value) {
    m_up_thickness = value;
  }

  inline void setDownThickness(float value) {
    m_down_thickness = value;
  }

  inline void setNeuronUpThickness(float value) {
    m_neuron_up_thickness = value;
  }

  inline void setNeuronDownThickness(float value) {
    m_neuron_down_thickness = value;
  }

public: // Commands -- vertices

  inline void appendVertex(const sl::point2f& p) {
    m_allpoints.push_back(p);
    m_bounding_box_up_to_date = false;
  }

  inline void setVertex(int i, const sl::point2f& p) {
    assert(goodVertexIndex(i));
    m_allpoints[i] = p;
    m_bounding_box_up_to_date = false;
  }

public: // Commands -- neurons

  void appendNeuron(int tp, const sl::point2f& p);

  inline void setNeuron(int tp, int i, const sl::point2f& p) {
    assert(goodNeuronIndex(tp, i));
    m_allneurons[tp][i] = p;
  }

public: // Queries -- Sub regions

  sl::tuple2i getRegion(const sl::point2f& p) const;
  int getRegionId(int i, int j) const;


public: // Commands -- Sub regions

  void assignNeurons();

  void setRegionId(int i, int j, int id);

public: // Commands -- Intersection

  inline void setRegularSection(bool b) {
    m_regular_section = b;
  }


public: // Commands -- Geometry

  void transform(const sl::affine_map2f& xform);

public: // Commands -- Triangulation

  void triangulateSection();

public:  // Ghost Section

  inline void setGhostSection(const bool b) {
    m_ghost_section = b;
  }

  inline bool ghostSection() const {
    return m_ghost_section;
  }


protected:

  void computeBoundingBox();

protected:
  
  float                                     m_quota;
  float                                     m_up_thickness;
  float                                     m_down_thickness;
  float                                     m_neuron_up_thickness;
  float                                     m_neuron_down_thickness;

  std::vector<sl::point2f>                  m_allpoints;
  std::vector< std::vector<sl::point2f> >   m_allneurons;
  
  std::vector<sl::tuple3i>                  m_triangles;

  std::vector<bsCortexRegion*>              m_cortex_regions;

  bool                                      m_bounding_box_up_to_date;
  sl::aabox3f                               m_bounding_box;

  bool                                      m_regular_section;
  bool                                      m_ghost_section;
};


// Output

std::ostream& operator<< (std::ostream& os, const bsSection& s);

#endif // _BS_SECTION_H_

