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
// .NAME bsCortexRegion


#ifndef _BS_CORTEXREGION_H_
#define _BS_CORTEXREGION_H_

#include "bsRegion.h"

class bsSubRegion;


class bsCortexRegion : public bsRegion {

public: // Creation & Destruction

  bsCortexRegion(bsSection* s) {
    m_internal_boundary_index = -1;
    m_section = s;
  }

  virtual ~bsCortexRegion() {
  };

public: // Queries -- external boundary vertices

  virtual int externalBoundaryVertexCount() const {
    return m_external_boundary_indexes.size();
  }

  virtual int externalBoundaryVertexIndex(int i) const {
    assert(goodExternalBoundaryVertexIndex(i));
    return  m_external_boundary_indexes[i];
  }

public: // Queries -- internal boundary

  inline bool hasInternalBoundary() const {
    return goodHoleIndex(m_internal_boundary_index);
  }

  inline int getInternalBoundaryIndex() const {
    assert(hasInternalBoundary());
    return m_internal_boundary_index;
  }
  
  inline bool goodInternalBoundaryVertexIndex(int i) const {
    return hasInternalBoundary() &&
      goodHoleVertexIndex(getInternalBoundaryIndex(), i);
  }

  inline int internalBoundaryVertexCount() const {
    int result = 0;
    if (hasInternalBoundary()) {
      result = holeVertexCount(getInternalBoundaryIndex());
    }
    return result;
  }

  inline int internalBoundaryVertexIndex(int i) const {
    assert(goodInternalBoundaryVertexIndex(i));
    return holeVertexIndex(getInternalBoundaryIndex(), i);
  }

   inline sl::point2f internalBoundaryVertexItem(int i) const {
     assert(goodInternalBoundaryVertexIndex(i));
     return holeVertexItem(getInternalBoundaryIndex(), i);
  }

   inline sl::point3f internalBoundaryVertexItem3(int i) const {
     assert(goodInternalBoundaryVertexIndex(i));
     return holeVertexItem3(getInternalBoundaryIndex(), i);
  }
 

public: // Queries -- SubRegion

  bsSubRegion* newSubRegion(const sl::tuple2i& cut1,
			    const sl::tuple2i& cut2) const;

  inline int subRegionCount() const {
    return m_sub_regions.size();
  }

  inline bool goodSubRegionIndex(int i) const {
    return i>= 0 && i< subRegionCount();
  }

  inline const bsSubRegion* subRegionItem(int i) const {
    assert(goodSubRegionIndex(i));
    return m_sub_regions[i];
  }

  inline bsSubRegion* subRegionItem(int i) {
    assert(goodSubRegionIndex(i));
    return m_sub_regions[i];
  }

  int subRegionId(int i) const;

  int maxSubRegionId() const;

  float subRegionItemArea(int s) const;
  float subRegionIdArea(int id) const;

public: // Query -- Geometry

  int searchSubRegionWithPoint(const sl::point2f& p) const;

public: // Commands -- Sub regions

  virtual void appendNeuronIndex(int tp, int i);

  virtual void appendNeuronIndex(int tp, int i, int sub_region_id);

  void makeSubRegions(const std::vector<sl::tuple2i>& cuts,
		      const std::vector<int>& region_ids); 

  void appendSubRegion( int id,
			std::vector< int >& external_contour,
			std::vector< int >& internal_contour,
			std::vector< int >& border1,
			std::vector< int >& border2,
			std::vector< std::vector<int> >& hole);

  virtual void insertHole(const std::vector<int>& h); 

  virtual void assignNeurons();

  void setSubRegionId(int i, int id);

public: // Commands

  virtual void clear(); 
  virtual void clearNeurons(); 
  virtual void clearHoles(); 

  void setExternalBoundary(const std::vector<int>& c);

  void setInternalBoundaryIndex(int i) {
    assert(goodHoleIndex(i));
    m_internal_boundary_index = i;
  }

  virtual bool triangulateRegion();

protected:
  std::vector<int>             m_external_boundary_indexes;
  
  int                          m_internal_boundary_index;

  std::vector<bsSubRegion*>    m_sub_regions;

};

std::ostream& operator<< (std::ostream& os, const bsCortexRegion& s);

#endif // _BS_CORTEXREGION_H_


