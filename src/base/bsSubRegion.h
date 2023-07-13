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
// .NAME bsSubRegion


#ifndef _BS_SUBREGION_H_
#define _BS_SUBREGION_H_

#include "bsRegion.h"

class bsCortexRegion;

class bsSubRegion : public bsRegion {

public: // Creation & Destruction

  bsSubRegion(const bsCortexRegion*,
	      const sl::tuple2i& c1,
	      const sl::tuple2i& c2);

  bsSubRegion(const bsCortexRegion* f,
	      std::vector< int >& external_contour,
	      std::vector< int >& internal_contour,
	      std::vector< int >& border1,
	      std::vector< int >& border2,
	      std::vector< std::vector<int> >& hole);

  virtual ~bsSubRegion();

  virtual bool invariant() const;

public: // Queries -- external contour vertices

  inline int externalContourVertexCount() const {
    return m_external_contour_indexes.size();
  }

  inline bool goodExternalContourVertexIndex(int i) const {
     return i>=0 && i< externalContourVertexCount();
  }

  inline int externalContourVertexIndex(int i) const {
    assert(goodExternalContourVertexIndex(i));
    return  m_external_contour_indexes[i];
  }

  sl::point2f externalContourVertexItem(int i) const;

  sl::point3f externalContourVertexItem3(int i) const;

public: // Queries -- internal contour vertices

  inline int internalContourVertexCount() const {
    
    return m_internal_contour_indexes.size();
  }

  inline bool goodInternalContourVertexIndex(int i) const {
     return i>=0 && i< internalContourVertexCount();
  }

  inline int internalContourVertexIndex(int i) const {
    assert(goodInternalContourVertexIndex(i));
    return  m_internal_contour_indexes[i];
  }

  sl::point2f internalContourVertexItem(int i) const;

  sl::point3f internalContourVertexItem3(int i) const;

public: // Queries -- border1 vertices

  inline int border1VertexCount() const {
    
    return m_border1_indexes.size();
  }

  inline bool goodBorder1VertexIndex(int i) const {
     return i>=0 && i< border1VertexCount();
  }

  inline int border1VertexIndex(int i) const {
    assert(goodBorder1VertexIndex(i));
    return  m_border1_indexes[i];
  }

  sl::point2f border1VertexItem(int i) const;

  sl::point3f border1VertexItem3(int i) const;

public: // Queries -- border2 vertices

  inline int border2VertexCount() const {
    
    return m_border2_indexes.size();
  }

  inline bool goodBorder2VertexIndex(int i) const {
     return i>=0 && i< border2VertexCount();
  }

  inline int border2VertexIndex(int i) const {
    assert(goodBorder2VertexIndex(i));
    return  m_border2_indexes[i];
  }

  sl::point2f border2VertexItem(int i) const;

  sl::point3f border2VertexItem3(int i) const;

public: // Queries -- external boundary vertices

  virtual int externalBoundaryVertexCount() const {
    return  
      externalContourVertexCount()+
      internalContourVertexCount()+
      border1VertexCount()+
      border2VertexCount()-
      4;
  }

  virtual int externalBoundaryVertexIndex(int i) const {
    assert(goodExternalBoundaryVertexIndex(i));
    int ec=externalContourVertexCount()-1;
    int ic=internalContourVertexCount()-1;
#ifndef NDEBUG
    int b1c=border1VertexCount()-1;
#endif
    int b2c=border2VertexCount()-1;
    int result;
    int j;

    if(i<ec){
      j=i;
      result=externalContourVertexIndex(j);
    } else if(i<ec+b2c){
      j=i-ec;
      result=border2VertexIndex(j);
    } else if(i<ec+b2c+ic){
      j=i-(ec+b2c);
      result=internalContourVertexIndex(j);
    } else {
      assert(i<ec+b2c+ic+b1c);
      j=i-(ec+b2c+ic);
      result=border1VertexIndex(j);
    }
    return  result;
  }


public: // Queries -- Id

  inline int getId() const {
    return m_id;
  }

public: // Queries -- Geometry

    bool isInside(const sl::point2f& p) const;

public: // Commands 

  inline void setId(int id) {
    m_id = id;
  }


protected: // Commands

  virtual void clear() {
    bsRegion::clear();
    m_external_contour_indexes.clear();
    m_internal_contour_indexes.clear();
    m_border1_indexes.clear();
    m_border2_indexes.clear();

  }

protected:
  int                     m_id;
  std::vector<int>             m_external_contour_indexes;
  std::vector<int>             m_internal_contour_indexes;
  std::vector<int>             m_border1_indexes;
  std::vector<int>             m_border2_indexes;

};

std::ostream& operator<< (std::ostream& os, const bsSubRegion& s);

#endif // _BS_SUBREGION_H_

