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
#ifndef _BS_GEOMETRYHELPERS_H_
#define _BS_GEOMETRYHELPERS_H_

#include <sl/fixed_size_point.hpp>
#include <vector>


class bsGeometryHelpers {
public:

  class polygonIntersection {
  public:
    inline polygonIntersection() {
      m_closestVertexIndex = -1;
      m_rayParameter = 1E30;
    }
    inline polygonIntersection(int i, float f) {
      m_closestVertexIndex = i;
      m_rayParameter = f;
    }
    inline int closestVertexIndex() const {
      return m_closestVertexIndex;
    }
    inline float rayParameter() const {
      return m_rayParameter;
    }
  public:
    int m_closestVertexIndex;
    float m_rayParameter;
  };

public:

  static bool goodConnectivity(const std::vector<int>& c,
			       const std::vector<sl::point2f>& p);

  static float signedArea(const std::vector<int>& c,
			  const std::vector<sl::point2f>& p);
  
  static bool isInside(const std::vector<int>& c,
		       const std::vector<sl::point2f>& p,
		       const sl::point2f& P);
  
  static bool isInside(const std::vector<int>& c1,
		       const std::vector<sl::point2f>& p1,
		       const std::vector<int>& c2,
		       const std::vector<sl::point2f>& p2);

  static bool isCCW(const std::vector<int>& c,
		    const std::vector<sl::point2f>& p);

  static std::vector<sl::tuple3i> triangulation(const std::vector<int>& c,
					 const std::vector<sl::point2f>& points);

  static std::pair<int, sl::point2f> triangleCoordinates(const std::vector<sl::tuple3i>& triangulation,
						  const std::vector<sl::point2f>& points,
						  const sl::point2f& P);

  static polygonIntersection intersect(const std::vector<int>& c,
				       const std::vector<sl::point2f>& p,
				       const sl::point2f& o,
				       const sl::point2f& e);

  static int closestPoint(const std::vector<int>& c,
			  const std::vector<sl::point2f>& p,
			  const sl::point2f& P);

  static sl::point2f cartToBary(const sl::point2f& P,
			const sl::point2f& P1, 
			const sl::point2f& P2, 
			const sl::point2f& P3); 

  static bool goodTriangle(const sl::point2f& P1,
			   const sl::point2f& P2,
			   const sl::point2f& P3);

  static sl::point2f baryToCart(const sl::point2f& uv,
			const sl::point2f& P1, 
			const sl::point2f& P2, 
			const sl::point2f& P3);
  
  static sl::color3f rgb2hsv(const sl::color3f& rgb);
  static sl::color3f hsv2rgb(const sl::color3f& hsv);
  static float luminance(const sl::color3f& rgb);
  static sl::color3f createColor(int i);

  static sl::point2f findInternalPoint(const std::vector<int>& c,
				     const std::vector<sl::point2f>& p);
    
  
}; // bsGeometryHelpers

#endif // _BS_GEOMETRYHELPERS_H_


