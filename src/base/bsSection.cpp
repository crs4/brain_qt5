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
#include "bsSection.h"
#include "bsGeometryHelpers.h"
#include "bsCortexRegion.h"
#include "bsSubRegion.h"


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


// queries -- SubRegion

int bsSection::maxSubRegionId() const {
  int result = -1;
  {
    for (int i=0; i<cortexRegionCount(); i++) {
      int id=cortexRegionItem(i)->maxSubRegionId();
      if (id > result) result = id;
    }
  }
  return result;
}


// Queries -- Polygons

bool bsSection::isCCW(const std::vector<int>& c) const {
  return bsGeometryHelpers::isCCW(c,m_allpoints);
}
    
float bsSection::signedArea(const std::vector<int>& c) const {
  return bsGeometryHelpers::signedArea(c,m_allpoints);
}

bool bsSection::isInside(const std::vector<int>& c,
	      const sl::point2f& P) const {
  return bsGeometryHelpers::isInside(c,m_allpoints,P);
}

bool  bsSection::isInside(const std::vector<int>& c1,
			  const std::vector<int>& c2) const {
  return bsGeometryHelpers::isInside(c1,m_allpoints,c2,m_allpoints);
}


// Creation & Destruction

bsSection::bsSection() {
  m_bounding_box_up_to_date = false;
  m_up_thickness=0.0f;
  m_down_thickness=0.0f;
  m_neuron_up_thickness=0.0f;
  m_neuron_down_thickness=0.0f;
  m_quota=0.0f;
  m_regular_section=true;
  m_ghost_section=false;
}

bsSection::~bsSection() {
  clear();
}

bsSection::bsSection(const std::vector< sl::point2f >&   points,
		     const std::vector< std::vector<sl::point2f> >&   allneurons,
		     const float                         quota) {

  m_bounding_box_up_to_date = false;
  m_up_thickness=0.0f;
  m_down_thickness=0.0f;
  m_neuron_up_thickness=0.0f;
  m_neuron_down_thickness=0.0f;
  m_regular_section=true;
  m_ghost_section=false;
  setQuota(quota);

  // Add points
  {
    for (int i=0; i<(int)points.size(); i++) {
      appendVertex(points[i]);
    }
  }

   // Add neurons
  m_allneurons = allneurons;
}


bsSection::bsSection(const std::vector< sl::point2f >& points,
		     std::vector< int >                external_boundary_connectivity,
		     std::vector< int >                internal_boundary_connectivity,
		     std::vector< std::vector<int> >   holes_or_boundaries,
		     std::vector< sl::tuple2i >        cuts,
		     std::vector< int >                region_ids) {

  assert(cuts.size() == region_ids.size());
  assert(cuts.size() == 0 || cuts.size() > 1);

  m_up_thickness=0.0f;
  m_down_thickness=0.0f;
  m_neuron_up_thickness=0.0f;
  m_neuron_down_thickness=0.0f;
  m_bounding_box_up_to_date = false;
  m_regular_section=true;
  m_ghost_section=false;

  // Add points
  {
    for (int i=0; i<(int)points.size(); i++) {
      appendVertex(points[i]);
    }
  }

  // Check orientations, make everything CCW
  bool swapExternalBoundary = !isCCW(external_boundary_connectivity);
  if (swapExternalBoundary) {
    myreverse(external_boundary_connectivity.begin(),
	    external_boundary_connectivity.end());
  }
  assert(isCCW(external_boundary_connectivity));

  bool swapInternalBoundary = !isCCW(internal_boundary_connectivity);
  if (swapInternalBoundary) {
    myreverse(internal_boundary_connectivity.begin(),
	    internal_boundary_connectivity.end());
  }
  assert(isCCW(internal_boundary_connectivity));

  {
    for (int i=0; i<(int)holes_or_boundaries.size(); i++) {
      if (!isCCW(holes_or_boundaries[i])) {
	myreverse(holes_or_boundaries[i].begin(), holes_or_boundaries[i].end());
      }
      assert(isCCW(holes_or_boundaries[i]));
    }
  }

  if (cuts.size() > 0) {
    if (swapExternalBoundary || swapInternalBoundary) {
      int ext_up = external_boundary_connectivity.size()-1;
      int int_up = internal_boundary_connectivity.size()-1;
      for (int i=0; i<(int)cuts.size(); i++) {
	if (swapExternalBoundary) {
	  cuts[i][0] = ext_up - cuts[i][0];
	}
	if (swapInternalBoundary) {
	  cuts[i][1] = int_up - cuts[i][1];
	}
      }
    }

    // reorder cuts
    { 
      for (int n=cuts.size()-1; n>0; n--) {
	for (int i=0; i<n; i++) {
	  if (cuts[i][0] > cuts[i+1][0]) {
	    sl::tuple2i tmp= cuts[i];
	    cuts[i]=cuts[i+1];
	    cuts[i+1]=tmp;
	    int tmpid=region_ids[i];
	    region_ids[i]=region_ids[i+1];
	    region_ids[i+1]=tmpid;
	  }
	}
      }
    }
  }

  // Insert connectivity data
  {
    bsCortexRegion* r = new bsCortexRegion(this);
    r->setExternalBoundary(external_boundary_connectivity);
    r->insertHole(internal_boundary_connectivity);
    r->setInternalBoundaryIndex(0);

    // Make sub regions
    if (cuts.size() > 1) {
      r->makeSubRegions(cuts, region_ids);
    }

    m_cortex_regions.push_back(r);
  }

  {
    for (int i=0; i<(int)holes_or_boundaries.size(); i++) {
      if (bsGeometryHelpers::isInside(external_boundary_connectivity,
				      m_allpoints,
				      holes_or_boundaries[i],
				      m_allpoints) &&
	  !bsGeometryHelpers::isInside(internal_boundary_connectivity,
				      m_allpoints,
				      holes_or_boundaries[i],
				       m_allpoints)) {
	m_cortex_regions[0]->insertHole(holes_or_boundaries[i]);
      } else {
	bsCortexRegion* r = new bsCortexRegion(this);
	r->setExternalBoundary(holes_or_boundaries[i]);
	m_cortex_regions.push_back(r);
      }
    }
  }

  // Triangulate section
  triangulateSection();

}


// Queries -- neurons

int bsSection::neuronCount() const {
  int result=0;
  for (int i=0; i< (int)m_allneurons.size(); i++) {
    result += neuronCount(i);
  }
  return result;
}


// Queries -- Cortex region

int bsSection::subRegionCount(int i) const {
  assert(goodCortexRegionIndex(i));
  return m_cortex_regions[i]->subRegionCount();
}


// Commands -- clear

void bsSection::clear() {
  m_bounding_box_up_to_date = false;

  for (int i=0; i<(int)m_cortex_regions.size(); i++) {
    delete m_cortex_regions[i];
    m_cortex_regions[i]=NULL;
  }
  m_cortex_regions.clear();
}


// Command --  cortex region

void bsSection::appendCortexRegion(std::vector< int >& external_boundary,
				   int internal_boundary,
				   std::vector< std::vector< int > >& hole,
				   std::vector< int >& id,
				   std::vector< std::vector< int > >& external_contour,
				   std::vector< std::vector< int > >& internal_contour,
				   std::vector< std::vector< int > >& border1,
				   std::vector< std::vector< int > >& border2,
				   std::vector< std::vector< std::vector< int > > >& hole_sub) {

    bsCortexRegion* r = new bsCortexRegion(this);
    r->setExternalBoundary(external_boundary);
    for (int i=0;i<(int)hole.size();i++) {
      r->insertHole(hole[i]);
    }
    if(internal_boundary >= 0) {
      r->setInternalBoundaryIndex(internal_boundary);
    }
    for (int i=0;i<(int)id.size();i++) {
      r->appendSubRegion(id[i],external_contour[i],
			 internal_contour[i],
			 border1[i],
			 border2[i],hole_sub[i]);
    }
    m_cortex_regions.push_back(r);
}
				   


// Commands -- neurons

void bsSection::appendNeuron(int tp, const sl::point2f& p) {
  assert(tp>=0);
  
  while (maxNeuronType() < tp) {
    std::vector<sl::point2f> empty;
    m_allneurons.push_back(empty);
  }
  m_allneurons[tp].push_back(p);
}

// Queries -- Sub regions

sl::tuple2i bsSection::getRegion(const sl::point2f& p) const{
    sl::tuple2i result; result = -1, -1;
    for (int i=0; i<cortexRegionCount() && result[0]<0; i++) {
	if (cortexRegionItem(i)->subRegionCount() == 0) {
	    if (cortexRegionItem(i)->isInsideExternalBoundary(p)) {
		result = i, -1;
	    }
	} else {
	    int j = cortexRegionItem(i)->searchSubRegionWithPoint(p);
	    if (j>=0) {
		result = i, j;
	    }
	}
    }
    return result;
}
  
int bsSection::getRegionId(int i, int j) const{
  assert(goodCortexRegionIndex(i));
  assert(cortexRegionItem(i)->goodSubRegionIndex(j));
  return m_cortex_regions[i]->subRegionItem(j)->getId();
}
  

// Commands -- Sub regions

void bsSection::assignNeurons() {
  {
      for (int i=0; i<(int)m_cortex_regions.size(); i++) {
	  m_cortex_regions[i]->clearNeurons();
      }  
  }
  {
      int cached_count = 0;
      int total_count = 0;

      sl::tuple2i old_rid; old_rid = -1, -1;
      for (int tp=0; tp<=maxNeuronType(); tp++) {
	  for (int i=0; i<neuronCount(tp); i++) {
	      total_count++;
	      sl::point2f p = m_allneurons[tp][i];
	      if ((old_rid[0] >= 0 && old_rid[1] >= 0) && 
		  (m_cortex_regions[old_rid[0]]->subRegionItem(old_rid[1])->isInsideExternalBoundary(p))) {
		  m_cortex_regions[old_rid[0]]->appendNeuronIndex(tp,i,old_rid[1]);
		  cached_count++;
	      } else {
		  sl::tuple2i rid = getRegion(p);
		  if (rid[0] >= 0) {
		      if (rid[1] >= 0) {
			  m_cortex_regions[rid[0]]->appendNeuronIndex(tp,i,rid[1]);
		      } else {
			  m_cortex_regions[rid[0]]->appendNeuronIndex(tp,i);
		      }		      
		  }
		  old_rid = rid;
	      }
	  }
      }
#ifndef NDEBUG
      std::cerr << "Neuron assignments: cached = " << cached_count << " - total = " << total_count << std::endl;
#endif
  }
}

void bsSection::setRegionId(int i, int j, int id) {
  assert(goodCortexRegionIndex(i));
  assert(cortexRegionItem(i)->goodSubRegionIndex(j));
  m_cortex_regions[i]->subRegionItem(j)->setId(id);
}


// Commands -- Geometry

void bsSection::transform(const sl::affine_map2f& xform) {
  {
    m_bounding_box_up_to_date = false;

    for (int i=0; i<vertexCount(); i++) {
      m_allpoints[i] = xform * m_allpoints[i];
    }
  }
  {
    for (int tp=0; tp<=maxNeuronType(); tp++) {
      for (int i=0; i<neuronCount(tp); i++) {
	m_allneurons[tp][i] = xform * m_allneurons[tp][i];
      }
    }
  }
}

void bsSection::computeBoundingBox() {
  int n = vertexCount();
  if (n == 0) {
    m_bounding_box.to(sl::point3f(0.,0.,0.));
  } else {
    sl::vector3f dzup, dzdown;
    dzup   = 0.0f, 0.0f, getUpThickness();
    dzdown = 0.0f, 0.0f, -getDownThickness();

    m_bounding_box.to_empty();
    for (int i=0; i<n; i++) {
      m_bounding_box.merge(vertexItem3(i) + dzup);
      m_bounding_box.merge(vertexItem3(i) + dzdown);
    }
  }
  m_bounding_box_up_to_date = true;
}

// triangulation

void bsSection::triangulateSection() {

  m_triangles.clear();
 
  int n_cortex = cortexRegionCount();
  m_regular_section=true;
  for (int i=0; i<n_cortex; i++) {
    bsCortexRegion *cortex_region = cortexRegionItem(i);
    bool ok = cortex_region->triangulateRegion();
    m_regular_section = m_regular_section && ok;

    // append triangles also into the section;
    // as default the triangles of the cortex region are added,
    // otherwise add explicitly the triangles of the sub regions.
    int n_triangles = cortex_region->triangleCount();
    {
      for (int i=0; i<n_triangles; i++) {
        sl::tuple3i tri = cortex_region->triangleItem(i);
        m_triangles.push_back(tri);
      }
    }

  }
}


float bsSection::triangleArea(int i) const {

  assert(hasTriangulation());

  sl::point2f v0 = triangleVertexItem(i,0);
  sl::point2f v1 = triangleVertexItem(i,1);
  sl::point2f v2 = triangleVertexItem(i,2);

  float area = 0.5*( v0[0]*(v1[1]-v2[1])+v1[0]*(v2[1]-v0[1])+v2[0]*(v0[1]-v1[1]));

  return area;
}


float bsSection::sectionArea() const {

  assert(hasTriangulation());

  float section_area = 0.;
  int n_cortex = cortexRegionCount();

  for (int i=0; i<n_cortex; i++) {
    section_area += cortexRegionItem(i)->regionArea();
  }

  return section_area;
}


float bsSection::sectionVolume() const {

  assert(hasTriangulation());

  float section_volume = sectionArea()*getThickness();

  return section_volume;

}


float bsSection::cortexRegionArea(int c) const {
  assert(hasTriangulation());
  assert(goodCortexRegionIndex(c));

  return  cortexRegionItem(c)->regionArea();
}


float bsSection::cortexRegionVolume(int c) const {
  return cortexRegionArea(c)*getThickness();
}


float bsSection::subRegionItemArea(int c, int s) const {
  assert(hasTriangulation());
  //assert(goodCortexRegionIndex(c));
  assert (c==0); //sub regions are (eventually) only on the 1st cortex region (no. 0)

  return  cortexRegionItem(c)->subRegionItemArea(s);
}

float bsSection::subRegionIdArea(int c, int id) const {
  assert(hasTriangulation());
  //assert(goodCortexRegionIndex(c));
  assert (c==0); //sub regions are (eventually) only on the 1st cortex region (no. 0)

  return  cortexRegionItem(c)->subRegionIdArea(id);
}


float bsSection::subRegionItemVolume(int c, int s) const {
  return subRegionItemArea(c,s)*getThickness();
}


float bsSection::subRegionIdVolume(int c, int id) const {
  return subRegionIdArea(c,id)*getThickness();
}


// Output

std::ostream& operator<< (std::ostream& os, const bsSection& s) {
  os << "# SECTION BEGIN" << std::endl;

  os << "# Points" << std::endl;
  {
    for (int i=0; i< s.vertexCount(); i++) {
      sl::point3f p = s.vertexItem3(i);
      os << i << ": (" << p[0] << ", " << p[1] << ", " << p[2] << ")\n";
    }
  }
  
  os << "# Neurons" << std::endl;
  {
    for (int tp=0; tp <= s.maxNeuronType(); tp++) {
      for (int i=0; i< s.neuronCount(tp); i++) {
	sl::point3f p = s.neuronItem3(tp, i);
	os << tp << " " << i << ": (" << p[0] << ", " << p[1] << ", " << p[2] << ")\n";
      }
    }
  }

  if(s.hasTriangulation()) {
    os << "# Triangles" << std::endl;
    {
      for (int i=0; i< s.triangleCount(); i++) {
	sl::tuple3i t = s.triangleItem(i);
	os << i << ": (" << t[0] << ", " << t[1] << ", " << t[2] << ") " << 
	  "-- tri area: " << s.triangleArea(i) << std::endl;
      }
    }
    os << "# Section Measures" << std::endl;
    os << "Area:   " << s.sectionArea() << std::endl;
    os << "Volume: " << s.sectionVolume() << std::endl;
  }


  os << "# CORTEX REGION BEGIN" << std::endl;
  {
    os << "# Cortex Regions Count: " << s.cortexRegionCount() << std::endl;
    for (int i=0; i< s.cortexRegionCount(); i++) {
      os << *s.cortexRegionItem(i); 
    }
  }
  os << "# CORTEX REGION END" << std::endl << std::endl;

  os << "# SECTION END" << std::endl;
  os << std::endl;
  return os;
}

