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
#include "bsCortexRegion.h"
#include "bsSubRegion.h"
#include "bsSection.h"
#include "bsGeometryHelpers.h"


bsSubRegion* bsCortexRegion::newSubRegion(const sl::tuple2i& cut1,
					const sl::tuple2i& cut2) const {
  assert(goodExternalBoundaryVertexIndex(cut1[0]));
  assert(goodInternalBoundaryVertexIndex(cut1[1]));
  assert(goodExternalBoundaryVertexIndex(cut2[0]));
  assert(goodInternalBoundaryVertexIndex(cut2[1]));

  // assert( no self intersection )

  return new bsSubRegion(this,
			 cut1,
			 cut2);
}

// queries -- SubRegion

int bsCortexRegion::maxSubRegionId() const {
  int result = -1;
  {
    for (int i=0; i<subRegionCount(); i++) {
      int id=subRegionId(i);
      if (id > result) result = id;
    }
  }
  return result;
}

int bsCortexRegion::subRegionId(int i) const {
  assert(goodSubRegionIndex(i));
  return m_sub_regions[i]->getId();
}


float bsCortexRegion::subRegionItemArea(int s) const {
  assert(goodSubRegionIndex(s));

  return subRegionItem(subRegionId(s))->regionArea();

}


float bsCortexRegion::subRegionIdArea(int id) const {
  assert(goodSubRegionIndex(id));

  return subRegionItem(id)->regionArea();

}




// Query -- Geometry

int  bsCortexRegion::searchSubRegionWithPoint(const sl::point2f& p) const{
    int result = -1;
    for (int i=0; i<subRegionCount() && result<0; i++) {
	if(subRegionItem(i)->isInside(p)){
	    result=i;
	}
    }
    return result;
}

void bsCortexRegion::insertHole(const std::vector<int>& h) {
  assert(m_section->isCCW(h));
  bsRegion::insertHole(h);

  for (int s=0; s<subRegionCount(); s++) {
      bsSubRegion *sub_region = subRegionItem(s);
      if (bsGeometryHelpers::isInside(sub_region->externalBoundaryConnectivity(),
				      m_section->vertices(),
				      h,
				      m_section->vertices())) {
	  sub_region->insertHole(h);
      }
  }
}

// commands

void bsCortexRegion::appendNeuronIndex(int tp, int i) {
    assert(m_section->goodNeuronIndex(tp,i));
    bsRegion::appendNeuronIndex(tp,i);

    bsSubRegion *sub_region = NULL;
    for (int s=0; s<(int)m_sub_regions.size() && !sub_region; s++) {
	if (bsGeometryHelpers::isInside(subRegionItem(s)->externalBoundaryConnectivity(),
					m_section->vertices(),
					m_section->neuronItem(tp,i))) {
	    sub_region = subRegionItem(s);
	}
    }
    if (sub_region) {
	sub_region->appendNeuronIndex(tp,i);
    }
}

void bsCortexRegion::appendNeuronIndex(int tp, int i, int sub_region_id) {
    assert(m_section->goodNeuronIndex(tp,i));
    assert(goodSubRegionIndex(sub_region_id));
    bsRegion::appendNeuronIndex(tp,i);
    subRegionItem(sub_region_id)->appendNeuronIndex(tp,i);
}


void bsCortexRegion::setExternalBoundary(const std::vector<int>& c) {
  assert(m_section->isCCW(c));
  m_external_boundary_indexes = c;
}
  
void bsCortexRegion::clear() {
  bsRegion::clear();
  m_external_boundary_indexes.clear();
  m_internal_boundary_index = -1;
  for (int i=0; i<(int)m_sub_regions.size(); i++) {
    delete m_sub_regions[i];
    m_sub_regions[i]=NULL;
  }
  m_sub_regions.clear();
}

void bsCortexRegion::clearNeurons() {
  bsRegion::clearNeurons();
  for (int i=0; i<(int)m_sub_regions.size(); i++) {
    m_sub_regions[i]->clearNeurons();
  }
}

void bsCortexRegion::clearHoles() {
  bsRegion::clearHoles();
  for (int i=0; i<(int)m_sub_regions.size(); i++) {
    m_sub_regions[i]->clearHoles();
  }
}

void bsCortexRegion::makeSubRegions(const std::vector<sl::tuple2i>& cuts,
				    const std::vector<int>& region_ids) {

  assert(cuts.size() > 1);
  assert(region_ids.size() == cuts.size());

  {
    for (int i=0; i<(int)m_sub_regions.size(); i++) {
      delete m_sub_regions[i];
      m_sub_regions[i]=NULL;
    }
  }
  m_sub_regions.clear();

  {
    int n = cuts.size();
    for (int i=0; i<n; i++) {
      sl::tuple2i c0 = cuts[i];
      sl::tuple2i c1 = cuts[(i+1) % n];
      
      bsSubRegion *r = new bsSubRegion(this, c0, c1);
      r->setId(region_ids[i]);
      m_sub_regions.push_back(r);
    }
  }
 
  assert(subRegionCount() == (int)cuts.size());
}


void bsCortexRegion::appendSubRegion( int id,
				      std::vector< int >& external_contour,
				      std::vector< int >& internal_contour,
				      std::vector< int >& border1,
				      std::vector< int >& border2,
				      std::vector< std::vector<int> >& hole) {

  bsSubRegion* r = new bsSubRegion(this, external_contour, internal_contour, border1, border2, hole);
  r->setId(id);
  m_sub_regions.push_back(r);
}


void bsCortexRegion::assignNeurons() {
  bsRegion::assignNeurons();
  for (int i=0; i<subRegionCount(); i++) {
    m_sub_regions[i]->assignNeurons();
  }
}

void bsCortexRegion::setSubRegionId(int i, int id) {
  assert(goodSubRegionIndex(i));
  m_sub_regions[i]->setId(id);
}

// Triangulation

bool bsCortexRegion::triangulateRegion() {

  bool is_triangulation_ok = bsRegion::triangulateRegion();

  int n_sub = subRegionCount();
  for (int s=0; s<n_sub; s++) {
    bsSubRegion *sub_region = subRegionItem(s);
    bool sub_ok = sub_region->triangulateRegion();
    is_triangulation_ok = is_triangulation_ok && sub_ok;
  }
  return is_triangulation_ok;
}


// Output

std::ostream& operator<< (std::ostream& os, const bsCortexRegion& s) {
  os << "####### CortexRegion begin" << std::endl;
  
  os << (bsRegion&)s; 

  if (s.hasInternalBoundary()) {
    os << "# Internal Boundary - nh=" << s.getInternalBoundaryIndex() << std::endl;
    {  
      for (int i=0; i< s.internalBoundaryVertexCount(); i++) {
	os << s.internalBoundaryVertexIndex(i) << " ";
      }
      os << std::endl;
    }
  }
  os << "# Sub Regions begin" << std::endl;
  {
    os << "# Sub Regions Count: " << s.subRegionCount() << std::endl;
    for (int i=0; i< s.subRegionCount(); i++) {
      os << *s.subRegionItem(i); 
    }
  }
  os << "# Sub Regions end" << std::endl << std::endl;

  os << "####### CortexRegion end" << std::endl << std::endl;

  return os;
}


