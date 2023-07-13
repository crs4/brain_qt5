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
// .NAME bsRegion

#ifndef _BS_REGION_H_
#define _BS_REGION_H_

#include <vector>
#include <iostream>
#include <cassert>

#include <sl/fixed_size_point.hpp>

class bsSection;
struct triangulateio;

class bsRegion
{
protected:
  bsRegion() {
    m_section = NULL;
  }
    
public: // Creation & Destruction

  bsRegion(bsSection* s) {
    m_section = s;
  }

  virtual ~bsRegion() {
  }

  virtual bool invariant() const {
    return true; // to be defined
  }

public: // Queries -- parent section

  const bsSection* getParentSection() const {
    return m_section;
  }

  bsSection* getParentSection() {
    return m_section;
  }

public: // Queries -- vertices

  virtual int externalBoundaryVertexCount() const = 0; 

  inline bool goodExternalBoundaryVertexIndex(int i) const {
    return i>=0 && i< externalBoundaryVertexCount();
  }

  virtual int externalBoundaryVertexIndex(int i) const = 0;

  virtual std::vector<int> externalBoundaryConnectivity() const;

  sl::point2f externalBoundaryVertexItem(int i) const;
  sl::point3f externalBoundaryVertexItem3(int i) const;


public: // Queries -- neurons 

  int neuronCount(int tp) const;

  int neuronCount() const;

  inline int maxNeuronType() const {
    return m_neuron_indexes.size()-1;
  }

  inline bool goodNeuronIndex(int tp, int i) const {
    return i>=0 && i<neuronCount(tp);
  }

  inline int neuronIndex(int tp, int i) const {
    assert(goodNeuronIndex(tp,i));
    return m_neuron_indexes[tp][i];
  }

  sl::point2f neuronItem(int tp, int i) const;

  sl::point3f neuronItem3(int tp, int i) const;

public: // Queries -- Holes

  inline int holeCount() const {
    return m_holes.size();
  }

  inline bool goodHoleIndex(int nh) const {
    return nh>=0 && nh<holeCount();
  }

  inline int holeVertexCount(int nh) const {
    assert(goodHoleIndex(nh));
    return m_holes[nh].size();
  }

  inline bool goodHoleVertexIndex(int nh, int i) const {
     return i>=0 && i<holeVertexCount(nh);
  }

  inline int holeVertexIndex(int nh, int i) const {
     assert(goodHoleVertexIndex(nh,i));
     return m_holes[nh][i];
  }

  virtual std::vector<int> holeConnectivity(int nh) const;

  sl::point2f holeVertexItem(int nh, int i) const;

  sl::point3f holeVertexItem3(int nh, int i) const;

public: // Geometric queries

  bool isInsideExternalBoundary(const sl::point2f& p) const;

  bool isInside(const sl::point2f& p) const;

public: // Queries -- triangles

  inline int triangleCount() const {
    return m_triangles.size();
  }

  inline bool hasTriangulation() const {
    return triangleCount()>0;
  }

  inline bool goodTriangleIndex(int i) const {
    assert(hasTriangulation());
    return i>=0 && i< triangleCount();
  }

  inline sl::tuple3i triangleItem(int i) const {
    assert(hasTriangulation());
    assert(goodTriangleIndex(i));
    return m_triangles[i];
  }

  sl::point2f triangleVertexItem(int i, int v) const;

  sl::point3f triangleVertexItem3(int i, int v) const;

  std::vector<sl::tuple3i> getTriangles() const {
    return m_triangles;
  }


public: // Queries -- areas and volumes

  float triangleArea(int i) const;

  float regionArea() const;

  float regionVolume() const;


public: // Commands

  virtual void clear() {
    clearNeurons();
    clearHoles();
  }

  virtual void clearNeurons() {
    m_neuron_indexes.clear();
  }

  virtual void appendNeuronIndex(int tp, int i);

  virtual void clearHoles() {
    m_holes.clear();
  }
      
  virtual void assignNeurons();

  virtual void insertHole(const std::vector<int>& h); 

  virtual bool triangulateRegion();

protected: // Commands for autointersection

  bool reconstruct_contours(const struct triangulateio &in,
			    const struct triangulateio &out,
			    const std::vector< std::vector<sl::tuple2i> > &in_indices);
  bool search_next_index(int i,
			 std::vector<sl::tuple2i> *out_tmp_indices,
			 std::vector< std::vector<sl::tuple2i> > *out_indices,
			 std::vector<int> *intersections,
			 const sl::tuple2i &t_in,
			 bool &new_vertex_state,
			 int &old_index, int &new_index, int n_points);

  bool search_new_index(int i,
			std::vector<sl::tuple2i> *out_tmp_indices,
			std::vector< std::vector<sl::tuple2i> > *out_indices,
			std::vector<int> *intersections,
			const sl::tuple2i &t_in,
			bool &new_vertex_state,
			int my_index, int &old_index, int &new_index, int n_points);


  void update_vectors(std::vector<sl::tuple2i> *out_tmp_indices,
		      std::vector< std::vector<sl::tuple2i> > *out_indices,
		      const sl::tuple2i &tuple, int i, int n);


  bool my_couple(std::vector<int> *v, int n, int &loop,
		 std::vector< std::vector<int> > *ni, int i);

  bool find_indices(int n,
		    std::vector <std::vector<int> > *v, int i,
		    sl::tuple2i &t);

protected: // Helpers

  bool isInside( const std::vector<int>& c, const sl::point2f& p) const; 

protected:

  bsSection*                      m_section;

  std::vector< std::vector<int> > m_neuron_indexes;

  std::vector< std::vector<int> > m_holes;

  std::vector<sl::tuple3i>        m_triangles;

};

std::ostream& operator<< (std::ostream& os, const bsRegion& s);

#endif // _BS_REGION_H_

