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
#include "bsRegion.h"
#include "bsSection.h"
#include "bsGeometryHelpers.h"

#include <vector>
#include <string.h>

#ifdef WIN32
using namespace std;
#endif

extern "C" {
#define REAL double
#define VOID int
#include "triangle.h"
};

// Internal functions

static void myreverse(std::vector<int>::iterator first, 
	       std::vector<int>::iterator last) {
  while (first != last) {
    --last;
    if (first != last) {
      int tmp = *first;
      *first = *last;
      *last = tmp;
      first++;
    }
  }
}


// Queries -- vertices

std::vector<int> bsRegion::externalBoundaryConnectivity() const {
  std::vector<int> c;
  int n = externalBoundaryVertexCount();
  for (int i=0; i<n; i++) {
    c.push_back(externalBoundaryVertexIndex(i));
  }
  return c;
}

sl::point2f bsRegion::externalBoundaryVertexItem(int i) const {
  assert(goodExternalBoundaryVertexIndex(i));
  return m_section->vertexItem(externalBoundaryVertexIndex(i));
}

sl::point3f bsRegion::externalBoundaryVertexItem3(int i) const {
  assert(goodExternalBoundaryVertexIndex(i));
  return m_section->vertexItem3(externalBoundaryVertexIndex(i));
}


// Queries -- neurons 

int bsRegion::neuronCount(int tp) const { 
  int result = 0;
  if (tp>=0 && tp < (int)m_neuron_indexes.size()) {
    result = m_neuron_indexes[tp].size();
  }
  return result;
}

int bsRegion::neuronCount() const {
  int result = 0;
  for (int i=0; i< (int)m_neuron_indexes.size(); i++) {
    result += neuronCount(i);
  }
  return result;
}

sl::point2f bsRegion::neuronItem(int tp, int i) const {
  int k = neuronIndex(tp,i);
  return m_section->neuronItem(tp,k);
}

sl::point3f bsRegion::neuronItem3(int tp, int i) const {
  int k = neuronIndex(tp,i);
  return m_section->neuronItem3(tp,k);
}


// Queries -- Holes

std::vector<int> bsRegion::holeConnectivity(int nh) const {
  assert(goodHoleIndex(nh));
  std::vector<int> c;
  int n = holeVertexCount(nh);
  for(int i=0;i<n;i++) {
    c.push_back(holeVertexIndex(nh,i));
  }
  return c;
}

sl::point2f bsRegion::holeVertexItem(int nh, int i) const {
  assert(goodHoleVertexIndex(nh,i));
  int k = holeVertexIndex(nh,i);
  return m_section->vertexItem(k);
}

sl::point3f bsRegion::holeVertexItem3(int nh, int i) const {
  assert(goodHoleVertexIndex(nh,i));
  int k = holeVertexIndex(nh,i);
  return  m_section->vertexItem3(k);
}


// Geometric queries

bool bsRegion::isInsideExternalBoundary(const sl::point2f& p) const {
  return m_section->isInside(externalBoundaryConnectivity(), p);
}

bool bsRegion::isInside(const sl::point2f& p) const {
  bool result = isInsideExternalBoundary(p);
  if (result) {
    for(int nh=0;nh<holeCount() && result;nh++) {
      result = !m_section->isInside(holeConnectivity(nh), p);
    }
  }
  return result;
}

// Triangle queries

sl::point2f bsRegion::triangleVertexItem(int i, int v) const {

  assert(hasTriangulation());
  assert(goodTriangleIndex(i));
  assert(v>=0 && v<3);

  return m_section->vertexItem(m_triangles[i][v]);
}


sl::point3f bsRegion::triangleVertexItem3(int i, int v) const {

  assert(hasTriangulation());

  assert(goodTriangleIndex(i));
  assert(v>=0 && v<3);
  return m_section->vertexItem3(m_triangles[i][v]);
}


float bsRegion::triangleArea(int i) const {

  assert(hasTriangulation());

  sl::point2f v0 = triangleVertexItem(i,0);
  sl::point2f v1 = triangleVertexItem(i,1);
  sl::point2f v2 = triangleVertexItem(i,2);

  float area = 0.5*( v0[0]*(v1[1]-v2[1])+v1[0]*(v2[1]-v0[1])+v2[0]*(v0[1]-v1[1]));

  return area;
}


float bsRegion::regionArea() const {

  assert(hasTriangulation());

  float region_area = 0.;

  int n_tri = triangleCount();

  for (int i=0; i<n_tri; i++) {
    region_area += triangleArea(i);
  }

  return region_area;

}


float bsRegion::regionVolume() const {

  assert(hasTriangulation()); {
      //  if(hasTriangulation()) {
    float thickness = m_section->getUpThickness()+m_section->getDownThickness();

    float region_volume = regionArea()*thickness;

    return region_volume;
  }
  return 0.0f;
}


// Commands
 
void bsRegion::appendNeuronIndex(int tp, int i) {
  assert(m_section->goodNeuronIndex(tp,i));

  while (maxNeuronType() < tp) {
    std::vector<int> empty;
    m_neuron_indexes.push_back(empty);
  }
  m_neuron_indexes[tp].push_back(i);
}


void bsRegion::assignNeurons() {
  m_neuron_indexes.clear();
  for (int tp=0; tp<=m_section->maxNeuronType(); tp++) {
    m_neuron_indexes.push_back(std::vector<int>());
    for (int i=0; i<m_section->neuronCount(tp); i++) {
      sl::point2f p = m_section->neuronItem(tp, i);
      if (isInsideExternalBoundary(p)) {
	m_neuron_indexes[tp].push_back(i);
      }
    }
  }
}

void bsRegion::insertHole(const std::vector<int>& h) {
  assert(m_section->isCCW(h));
  m_holes.push_back(h);
}


// ### Triangulation routine ###
bool bsRegion::triangulateRegion() {
  bool is_triangulation_ok = true;
  if (!m_triangles.empty()) {
    m_triangles.clear();
  }

  std::vector<int> region_connectivity;
  int n_tot_vert = 0;
  // compute total number of vertices of the region
  n_tot_vert += externalBoundaryVertexCount();
  /*
  if (hasInternalBoundary() ) {
    n_tot_vert += internalBoundaryVertexCount();
  }
  */
  int n_holes = holeCount();
  {
    for (int j=0; j<n_holes; j++) {
      n_tot_vert += holeVertexCount(j);
    }
  }

  // this is for autointersection calculation
  std::vector< std::vector<sl::tuple2i> > in_indices(n_holes+1);

  // triangulate with Triangle 
  struct triangulateio in, out;

  memset(&in,  0, sizeof(struct triangulateio));
  memset(&out, 0, sizeof(struct triangulateio));

  in.numberofpoints          = n_tot_vert;
  in.numberofsegments        = n_tot_vert;
  in.numberofholes           = n_holes;

  in.pointlist         = new double[in.numberofpoints*2];
  in.segmentlist       = new int[in.numberofsegments*2];
  in.holelist          = new double[in.numberofholes*2];

  out.pointlist         = NULL;
  out.segmentlist       = NULL;
  out.holelist          = NULL;

  int n_cumul = 0;
  unsigned int np = 0;
  int n_ext = externalBoundaryVertexCount();

  in_indices[0].resize(n_ext);
  {
    for (int i=0; i<n_ext; i++) {
      sl::point2f p = externalBoundaryVertexItem(i);
      assert(np == region_connectivity.size());
      region_connectivity.push_back(externalBoundaryVertexIndex(i));
      in.pointlist[np*2]   =   p[0];
      in.pointlist[np*2+1] =   p[1];
      in.segmentlist[np*2] =   np;
      in.segmentlist[np*2+1] = n_cumul+((i+1)%(n_ext));

      in_indices[0][i][0] = np;
      in_indices[0][i][1] = n_cumul+((i+1)%(n_ext));

      np++;
    }
    n_cumul += n_ext;
  }

  // when triangulating a cortical region, the internal boundary can be viewed as the 0th hole,
  // so (in that case) we treat the internal boundary among the holes.
  std::vector<sl::point2f> hole;
  std::vector<int> hole_connectivity;
  std::vector<sl::point2f> internal_hole(n_holes);

  for (int h=0; h<n_holes; h++) {
    // the hole 0th is the internal boundary, if any
    int n_hvc = holeVertexCount(h);
    in_indices[h+1].resize(n_hvc);
    {
      for (int i=0; i<n_hvc; i++) {
	sl::point2f p = holeVertexItem(h, i);
	int p_conn  = holeVertexIndex(h, i);
	hole.push_back(p);
	hole_connectivity.push_back(i);
	assert(np == region_connectivity.size());
	region_connectivity.push_back(p_conn);
	in.pointlist[np*2] =   p[0];
	in.pointlist[np*2+1] = p[1];
	in.segmentlist[np*2] = np;
	in.segmentlist[np*2+1] = n_cumul+((i+1)%(n_hvc));

	in_indices[h+1][i][0] = np;
	in_indices[h+1][i][1] = n_cumul+((i+1)%(n_hvc));

	np++;
      }
      n_cumul += n_hvc;
    }
    assert(hole.size() == hole_connectivity.size());
    internal_hole[h] = bsGeometryHelpers::findInternalPoint(hole_connectivity, hole);
#ifndef NDEBUG
    std::cerr << "Internal hole: " << internal_hole[h];
#endif
    hole.clear();
    hole_connectivity.clear();
  }

  {
    for (int i=0; i<n_holes; i++) {
      in.holelist[i*2+0] = internal_hole[i][0];
      in.holelist[i*2+1] = internal_hole[i][1];
    }
  }
#ifndef NDEBUG
  char triswitches[]="zp";
  //  triangulate ("zp", &in, &out, NULL);
#else
  char triswitches[]="zpQ";
#endif // NDEBUG
  triangulate (triswitches, &in, &out, NULL);

  if(in.numberofsegments != out.numberofsegments) {
    is_triangulation_ok=false;
#ifndef NDEBUG
    std::cerr << "###Intersections in the contours! - Not treated!!!" << std::endl; 
#endif
#if INTERSECTION
    if(!reconstruct_contours(in, out, in_indices)) {
      std::cerr << "   ### ERROR: not adjustable intersection case!" << std::endl;
    }
#endif
  }

  // TODO: update region_connectivity with new vertices
  
  int conn_size = region_connectivity.size();
  {
    for (int i=0; i<out.numberoftriangles; i++) {
      if(out.trianglelist[i*3+0]<conn_size &&
	 out.trianglelist[i*3+1]<conn_size &&
	 out.trianglelist[i*3+2]<conn_size) {
	int i0 = region_connectivity[out.trianglelist[i*3+0]];
	int i1 = region_connectivity[out.trianglelist[i*3+1]];
	int i2 = region_connectivity[out.trianglelist[i*3+2]];
	sl::tuple3i tp; tp=i0,i1,i2;
	m_triangles.push_back(tp);
      } else {
#ifndef NDEBUG
	std::cerr << " Triangle with new vertices not added!" << std::endl;
#endif
      }
    }
  }
  return is_triangulation_ok;
}


bool bsRegion::reconstruct_contours(const struct triangulateio &in,
				    const struct triangulateio &out,
				    const std::vector< std::vector<sl::tuple2i> > &in_indices) {

  int n_holes = holeCount();

  std::vector<sl::tuple2i> *out_tmp_indices = new std::vector<sl::tuple2i>;
  {
    for (int i=0; i<out.numberofsegments; i++) {
      sl::tuple2i t; t = (int)out.segmentlist[i*2], (int)out.segmentlist[i*2+1];
      out_tmp_indices->push_back(t);
    }
    assert((unsigned int)out.numberofsegments == out_tmp_indices->size());
  }

  // now we insert in the right place the new ordered out_indices 
  // (autointersection case) ...
  std::vector< std::vector<sl::tuple2i> > *out_indices = 
    new std::vector< std::vector<sl::tuple2i> >(n_holes+1);
  std::vector<int> *intersections = new std::vector<int>;

  {
    bool new_vertex_state = false;
    int old_index, new_index;
    for (int i=0; i<n_holes+1; i++) {
      unsigned int j=0;
      while (j<(in_indices)[i].size()) {
	sl::tuple2i t_in = (in_indices)[i][j];
	bool found = false;
	found = search_next_index(i, out_tmp_indices, out_indices, intersections, t_in, 
				  new_vertex_state, old_index, new_index, 
				  in.numberofpoints);
	if (!found) {
#ifndef NDEBUG
	  std::cerr << "Error: correct new index not found" << std::endl;
#endif
	  return false;
	}
	if (!new_vertex_state) j++;
      }
    }
  }

  assert(out_tmp_indices->size() == 0);

  std::vector< std::vector<int> > *new_indices = new std::vector< std::vector<int> >(n_holes+1);
  {
    for (int i=0; i<(n_holes+1); i++) {
      for (int j=0; j<(int)(*out_indices)[i].size(); j++) {
	(*new_indices)[i].push_back((*out_indices)[i][j][0]);
      }
    }
  }

#ifndef NDEBUG
  if (in.numberofpoints != out.numberofpoints) {
    std::cerr << "##### in.numberofpoints  = " << in.numberofpoints << std::endl;
    std::cerr << "##### out.numberofpoints = " << out.numberofpoints << std::endl;
    std::cerr << "##### in.numberofsegments  = " << in.numberofsegments << std::endl;
    std::cerr << "##### out.numberofsegments = " << out.numberofsegments << std::endl;

    {
      for (int i=0; i<in.numberofsegments; i++) {
	std::cerr << "in.segmentlist[" << 2*i<< "," << 2*i+1 << "] = " <<  
	  in.segmentlist[2*i] << "  " << in.segmentlist[2*i+1] << std::endl;
      }
    }
    {
      for (int i=0; i<out.numberofsegments; i++) {
	std::cerr << "out.segmentlist[" << 2*i << "," << 2*i+1 << "] = " <<  
	  out.segmentlist[2*i] << "  " << out.segmentlist[2*i+1] << std::endl;
      }
    }

    {
      for (int i=0; i<n_holes+1; i++) {
	for (int j=0; j<(int)(in_indices)[i].size(); j++) {
	  std::cerr << "in_indices[" << i << "][" << j << "] = " <<  
	    (in_indices)[i][j][0] << " " << (in_indices)[i][j][1] << std::endl;
	}
      }
    }
    {
      for (int i=0; i<(int)out_tmp_indices->size(); i++) {
	std::cerr << "out_tmp_indices[" << i << "] = " <<  
	  (*out_tmp_indices)[i][0] << " " << (*out_tmp_indices)[i][1] << std::endl;
      }
    }
    {
      for (int i=0; i<n_holes+1; i++) {
	for (int j=0; j<(int)(*out_indices)[i].size(); j++) {
	  std::cerr << "out_indices[" << i << "][" << j << "] = " <<  
	    (*out_indices)[i][j][0] << " " << (*out_indices)[i][j][1] << std::endl;
	}
      }
    }
    {
      for (int i=0; i<n_holes+1; i++) {
	for (int j=0; j<(int)(*new_indices)[i].size(); j++) {
	  std::cerr << "new_indices[" << i << "][" << j << "] = " <<  
	    (*new_indices)[i][j] << std::endl;
	}
      }
    }
    {
      for (int i=0; i<(int)intersections->size(); i++) {
	std::cerr << "intersections[" << i << "] = " <<  
	  (*intersections)[i] << std::endl;
      }
    }
  }
#endif

  // now we reorder old indices between new indices
  // (also nested new indices are treated)
  int n=0;
  for (int i=0; i<n_holes+1 && n<(int)intersections->size(); i++) {
    int loop=0;
    if (my_couple(intersections, n, loop, new_indices, i)) {
      n += 2+loop*2+1;
    } else {
#ifndef NDEBUG
      std::cerr << "Error: can't reorder indices" << std::endl;
#endif
      return false;
    }
  }

#ifndef NDEBUG
  {
    std::cerr << "New indices after reordering ..." << std::endl;
    for (int i=0; i<n_holes+1; i++) {
      for (int j=0; j<(int)(*new_indices)[i].size(); j++) {
	std::cerr << "new_indices[" << i << "][" << j << "] = " <<  
	  (*new_indices)[i][j] << std::endl;
      }
    }
  }
# endif

  delete [] out_indices;

  return true;
}


bool bsRegion::search_next_index(int i,
				 std::vector<sl::tuple2i> *out_tmp_indices,
				 std::vector< std::vector<sl::tuple2i> > *out_indices,
				 std::vector<int> *intersections,
				 const sl::tuple2i &t_in,
				 bool &new_vertex_state,
				 int &old_index, int &new_index, int n_points) {
  bool found = false;
  int my_index;

  if (!new_vertex_state) {
    for (unsigned int n=0; n<out_tmp_indices->size() && !found; n++) {
      sl::tuple2i t_out = (*out_tmp_indices)[n];
      if ((t_out[0] == t_in[0] && t_out[1] == t_in[1]) ||
	  (t_out[0] == t_in[1] && t_out[1] == t_in[0])) {
	new_vertex_state = false;
	update_vectors(out_tmp_indices, out_indices, 
		       t_in, i, n);
	found = true;
      }
    }
    if (!found) {
      my_index = t_in[0];
      found = search_new_index(i, out_tmp_indices, out_indices, intersections, t_in, 
			       new_vertex_state, my_index, old_index, new_index, 
			       n_points);
    }
  } // ### end new_vertex_state = false
  else { // if (new_vertex_state)
    my_index = intersections->back();
    for (unsigned int n=0; n<out_tmp_indices->size() && !found; n++) {
      sl::tuple2i t_out = (*out_tmp_indices)[n];
      if (t_out[0] == my_index && t_out[1] == old_index) {
	new_vertex_state = false;
	
	update_vectors(out_tmp_indices, out_indices,
		       t_out, i, n);
	found = true;
      } else if (t_out[1] == my_index && t_out[0] == old_index) {
	new_vertex_state = false;
	sl::tuple2i swap; swap= t_out[1], t_out[0];
	update_vectors(out_tmp_indices, out_indices, 
		       swap, i, n);
	found = true;
      }
    }
    if (!found) {
      found = search_new_index(i, out_tmp_indices, out_indices, intersections, t_in, 
			       new_vertex_state, my_index, old_index, new_index, 
			       n_points);
    }
  } // ### end new_vertex_state = true

  return found;
}


bool bsRegion::search_new_index(int i,
				std::vector<sl::tuple2i> *out_tmp_indices,
				std::vector< std::vector<sl::tuple2i> > *out_indices,
				std::vector<int> *intersections,
				const sl::tuple2i &t_in,
				bool &new_vertex_state,
				int my_index, int &old_index, int &new_index, int n_points) {

  bool found = false;
  for (unsigned int n=0; n<out_tmp_indices->size() && !found; n++) {
    sl::tuple2i t_out = (*out_tmp_indices)[n];
    if (t_out[0] == my_index && t_out[1]>=n_points /*hack*/) {
      old_index = t_in[1];
      new_index = t_out[1];
      intersections->push_back(new_index);
      new_vertex_state = true;
      update_vectors(out_tmp_indices, out_indices, t_out, i, n);
      found = true;
    } else if (t_out[1] == t_in[0] && t_out[0]>=n_points /*hack*/) {
      old_index = t_in[1];
      new_index = t_out[0];
      intersections->push_back(new_index);
      new_vertex_state = true;
      sl::tuple2i swap; swap= t_out[1], t_out[0];
      update_vectors(out_tmp_indices, out_indices, swap, i, n);
      found = true;
    }
  }
  if (!found) {
    std::cerr << "### SEGMENT NOT FOUND! Not regular contour." << std::endl;
  }
  
  return found;
}


bool bsRegion::my_couple(std::vector<int> *v, int n, int &loop,
			 std::vector< std::vector<int> > *ni, int i) {
  if ((*v)[n+1] != (*v)[n]) {
    if(my_couple(v, n+1, loop, ni, i)) {
      loop++;
    } else {
      return false;
    }
  }

  sl::tuple2i t;
  if(find_indices((*v)[n], ni, i, t)) {
    myreverse((*ni)[i].begin()+t[0], (*ni)[i].begin()+t[1]+1);
    return true;
  } else {
    return false;
  }
}


bool bsRegion::find_indices(int n,
				   std::vector <std::vector<int> > *v, int i,
				   sl::tuple2i &t) {
  bool found_first = false;
  int accum=0;

  for (unsigned int p=0; p<(*v)[i].size(); p++) {
    if ((*v)[i][p] == n) {
      if (!found_first) {
	t[0] = p;
	found_first = true;
      } else {
	t[1] = p;
      }
      accum++;
    }
  }

  if (accum==2) { // "enough" regular autointersection case
    return true;
  } else {        // complex autointersection case or inters. bewteen diff. contours 
    return false;
  }

}


void bsRegion::update_vectors(std::vector<sl::tuple2i> *out_tmp_indices,
			      std::vector< std::vector<sl::tuple2i> > *out_indices,
			      const sl::tuple2i &tuple, int i, int n) {

  (*out_indices)[i].push_back(tuple);
  std::vector<sl::tuple2i>::iterator it_n = out_tmp_indices->begin()+n;
  out_tmp_indices->erase(it_n);

}



std::ostream& operator<< (std::ostream& os, const bsRegion& s){

  os << "# external boundary" << std::endl;
  {
    for (int i=0; i< s.externalBoundaryVertexCount(); i++) {
      os << s.externalBoundaryVertexIndex(i) << " ";
    }
    os << std::endl;
  }

  os << "# Hole Count: " << s.holeCount() << std::endl; 
  {
    for (int nh=0; nh<s.holeCount(); nh++) {
      os << " Hole #" << nh << std::endl;
       for (int i=0; i<s.holeVertexCount(nh); i++) {
	 os << s.holeVertexIndex(nh,i) << " ";
       }
      os << std::endl; 
    }
  }

  os << "# Neuron Count: " << s.neuronCount() << std::endl; 
  {
    for (int tp=0; tp<=s.maxNeuronType (); tp++) {
      os << "# Neuron type " << tp << std::endl;
       for (int i=0; i<s.neuronCount(tp); i++) {
	 os << s.neuronIndex(tp,i) << " ";
       }
      os << std::endl; 
    }
  }

  if(s.hasTriangulation()) {
    os << "# Triangle Count: " << s.triangleCount() << std::endl;
    {
      for (int i=0; i< s.triangleCount(); i++) {
	sl::tuple3i t = s.triangleItem(i);
	os << i << ": (" << t[0] << ", " << t[1] << ", " << t[2] << ") " <<
          "-- tri area: " << s.triangleArea(i) << std::endl;
      }
      os << std::endl;
    }
    os << "# Region Measures" << std::endl;
    os << "Area:   " << s.regionArea() << std::endl;
    os << "Volume: " << s.regionVolume() << std::endl;
  }

  return os;
}


