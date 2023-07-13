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
#include "bsSubRegion.h"
#include "bsSection.h"
#include "bsCortexRegion.h"
#include "bsGeometryHelpers.h"

// Creation

bsSubRegion::~bsSubRegion() {
}

bsSubRegion::bsSubRegion(const bsCortexRegion* f,
			 const sl::tuple2i& cut1,
			 const sl::tuple2i& cut2) {
  assert(f);
  assert(f->goodExternalBoundaryVertexIndex(cut1[0]));
  assert(f->goodInternalBoundaryVertexIndex(cut1[1]));
  assert(f->goodExternalBoundaryVertexIndex(cut2[0]));
  assert(f->goodInternalBoundaryVertexIndex(cut2[1]));

  m_section = ((bsCortexRegion*)f)->getParentSection();

  // Boundary
  {
    int n = f->externalBoundaryVertexCount();
    for (int i= cut1[0]; i!= cut2[0]; i = (i+1) % n) {
      m_external_contour_indexes.push_back(f->externalBoundaryVertexIndex(i));
    }
    m_external_contour_indexes.push_back(f->externalBoundaryVertexIndex(cut2[0]));
  }

  m_border2_indexes.push_back(f->externalBoundaryVertexIndex(cut2[0]));
  m_border2_indexes.push_back(f->internalBoundaryVertexIndex(cut2[1]));

  {  
    int n = f->internalBoundaryVertexCount();
    for (int i= cut2[1]; i!= cut1[1]; i = (i-1 + n) % n) {
      m_internal_contour_indexes.push_back(f->internalBoundaryVertexIndex(i));
    }
    m_internal_contour_indexes.push_back(f->internalBoundaryVertexIndex(cut1[1]));
  }

  m_border1_indexes.push_back(f->internalBoundaryVertexIndex(cut1[1]));
  m_border1_indexes.push_back(f->externalBoundaryVertexIndex(cut1[0]));

  // Assign holes -- quick'n'dirty
  for (int nh=0; nh<f->holeCount(); nh++) {
    if (nh != f->getInternalBoundaryIndex()) {
      if (f->holeVertexCount(nh) > 0 &&
	  isInsideExternalBoundary(f->holeVertexItem(nh, 0))) {
	insertHole(f->holeConnectivity(nh));
      }
    }
  }

  // Assign neurons
  assignNeurons();

  assert(invariant());
}

bsSubRegion::bsSubRegion(const bsCortexRegion* f,
			 std::vector< int >& external_contour,
			 std::vector< int >& internal_contour,
			 std::vector< int >& border1,
			 std::vector< int >& border2,
			 std::vector< std::vector<int> >& hole) {

  m_section = ((bsCortexRegion*)f)->getParentSection();  
  m_external_contour_indexes = external_contour;
  m_internal_contour_indexes = internal_contour;
  m_border1_indexes = border1;
  m_border2_indexes = border2;
  m_holes = hole;

  // Assign neurons
  //  assignNeurons();

  assert(invariant()); 
}


bool bsSubRegion::invariant() const {
  return 
    bsRegion::invariant() &&
    externalContourVertexCount() >= 2 &&
    internalContourVertexCount() >= 2 &&
    border1VertexCount() >= 2 &&
    border2VertexCount() >= 2 &&
    externalContourVertexIndex(externalContourVertexCount()-1) == 
    border2VertexIndex(0) &&
    border2VertexIndex(border2VertexCount()-1) ==
    internalContourVertexIndex(0) &&
    internalContourVertexIndex(internalContourVertexCount()-1) ==
    border1VertexIndex(0) &&
    border1VertexIndex(border1VertexCount()-1) ==
    externalContourVertexIndex(0);
}    


// Queries -- external contour vertices

sl::point2f bsSubRegion::externalContourVertexItem(int i) const {
  assert(goodExternalContourVertexIndex(i));
  return m_section->vertexItem(externalContourVertexIndex(i));
}

sl::point3f bsSubRegion::externalContourVertexItem3(int i) const {
  assert(goodExternalContourVertexIndex(i));
  return m_section->vertexItem3(externalContourVertexIndex(i));
}

// Queries -- internal contour vertices

sl::point2f bsSubRegion::internalContourVertexItem(int i) const {
  assert(goodInternalContourVertexIndex(i));
  return m_section->vertexItem(internalContourVertexIndex(i));
}

sl::point3f bsSubRegion::internalContourVertexItem3(int i) const {
  assert(goodInternalContourVertexIndex(i));
  return m_section->vertexItem3(internalContourVertexIndex(i));
}

// Queries -- border1 vertices

sl::point2f bsSubRegion::border1VertexItem(int i) const {
  assert(goodBorder1VertexIndex(i));
  return m_section->vertexItem(border1VertexIndex(i));
}

sl::point3f bsSubRegion::border1VertexItem3(int i) const {
  assert(goodBorder1VertexIndex(i));
  return m_section->vertexItem3(border1VertexIndex(i));
}

// Queries -- border2 vertices

sl::point2f bsSubRegion::border2VertexItem(int i) const {
  assert(goodBorder2VertexIndex(i));
  return m_section->vertexItem(border2VertexIndex(i));
}

sl::point3f bsSubRegion::border2VertexItem3(int i) const {
  assert(goodBorder2VertexIndex(i));
  return m_section->vertexItem3(border2VertexIndex(i));
}


// Queries -- Geometry

bool bsSubRegion::isInside(const sl::point2f& p) const{
    std::vector<int> external_border;
    for (int i=0; i<externalBoundaryVertexCount(); i++) {
	external_border.push_back(externalBoundaryVertexIndex(i));
    }
    return m_section->isInside(external_border,p);
}



std::ostream& operator<< (std::ostream& os, const bsSubRegion& s) {
  os << "####### SubRegion begin - ID " << s.getId() << std::endl;
  
  os << (bsRegion&)s; 

  os << "####### SubRegion end" << std::endl << std::endl;

  return os;
}
