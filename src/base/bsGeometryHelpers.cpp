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
#include "bsGeometryHelpers.h"
#include <sl/math.hpp>
#include <iostream>
#include <cassert>
#include <string.h>

extern "C" {

#ifdef SINGLE
#define REAL float
#else 
#define REAL double
#endif

#include "triangle.h"
};

bool bsGeometryHelpers::goodConnectivity(const std::vector<int>& c,
					 const std::vector<sl::point2f>& p) {
  bool result = true;
  int n = c.size();
  for (int i=0; i<n && result; i++) {
    result = c[i] >= 0 && c[i] < (int)p.size();
  }
  return result;
}

float bsGeometryHelpers::signedArea(const std::vector<int>& c,
				    const std::vector<sl::point2f>& p) {
  assert(goodConnectivity(c,p));

  float result = 0.0f;

  int n = c.size();
  if (n > 2) {
    // Do the wrap-around first 
    sl::point2f v1 = p[c[n-1]];
    sl::point2f v0 = p[c[0]];
    
    double area2 = v1[0] * v0[1] - v0[0] * v1[1];

    // Compute the signed area (times 2) of the polygon
    for (int i = 0; i < n-1; i++) {
      v1 = p[c[i+1]];
      v0 = p[c[i]];
      area2 += v0[0] * v1[1] - v1[0] * v0[1];
    }
      
    result = 0.5 * area2;
  }
  return result;
}


bool  bsGeometryHelpers::isInside(const std::vector<int>& c,
				  const std::vector<sl::point2f>& p,
				  const sl::point2f& P) {
  assert(goodConnectivity(c,p));

  /* determine the quadrant of a polygon point
     relative to the test point */

#define quadrant(vertex, x, y) ( (vertex[0] > x) ? ((vertex[1] > y) ? 0 : 3) : ( (vertex[1] > y) ? 1 : 2) )

  /* determine x intercept of a polygon edge
     with a horizontal line at the y value of the test point */
#define x_intercept(pt1, pt2,  yy) (pt2[0] - ( (pt2[1] - yy) * ((pt1[0] - pt2[0]) / (pt1[1] - pt2[1])) ) )

  sl::point2f vertex = p[c[0]];
  short quad = quadrant(vertex, P[0], P[1]);
  short angle = 0;
  int n = c.size();
  for (int i=0; i<n; i++) {
    sl::point2f next_vertex =  p[c[(i+1)%n]];
    short     next_quad = quadrant(next_vertex, P[0], P[1]);
    short     delta = next_quad - quad;
    
    switch (delta) { 
      // make quadrant deltas wrap around   
    case  3:    delta = -1; break;                         
    case -3:    delta =  1; break;                         
      // check if went around point cw or ccw 
    case  2: 
    case -2: 
      if (x_intercept(vertex, next_vertex, P[1]) > P[0]) {
	delta =  - (delta); 
      }
      break;
    }

    angle = angle + delta; 
    quad = next_quad;
    vertex = next_vertex;
  }
  
  // complete 360 degrees (angle of + 4 or -4 ) means inside 
  return ((angle == +4) || (angle == -4));
  
  // odd number of windings rule 
  // if (angle & 4) return INSIDE; else return OUTSIDE; 
  // non-zero winding rule 
  // if (angle != 0) return INSIDE; else return OUTSIDE; 
}


bool  bsGeometryHelpers::isInside(const std::vector<int>& c1,
				  const std::vector<sl::point2f>& p1,
				  const std::vector<int>& c2,
				  const std::vector<sl::point2f>& p2) {
  assert(c2.size() > 0);
  assert(goodConnectivity(c1,p1));
  assert(goodConnectivity(c2,p2));
  
  //TO BE REWRITTEN !!!!
  
  return isInside(c1,p1,p2[c2[0]]);
}


bool  bsGeometryHelpers::isCCW(const std::vector<int>& c,
			       const std::vector<sl::point2f>& p) {
  assert(goodConnectivity(c,p));

  return signedArea(c,p) > 0.0f;
}

std::vector<sl::tuple3i> bsGeometryHelpers::triangulation(const std::vector<int>& c,
						   const std::vector<sl::point2f>& points) {
  std::vector<sl::tuple3i> result;

  // triangulate with Triangle 
  struct triangulateio in, out;

  memset(&in,  0, sizeof(struct triangulateio));
  memset(&out, 0, sizeof(struct triangulateio));

  in.numberofpoints          = c.size();
  in.numberofpointattributes = 0;
  in.numberofsegments        = c.size();
  in.numberofholes           = 0;
  in.numberofregions         = 0;

  in.pointlist         = new REAL[in.numberofpoints*2];
  in.pointmarkerlist   = NULL;
  in.segmentlist       = new int[in.numberofsegments*2];
  in.segmentmarkerlist = NULL;
  in.holelist          = NULL;
  in.regionlist        = NULL;

  {
      int n = c.size();
      for (int i=0; i<n; i++) {
	  assert(c[i] >= 0 && c[i] < (int)points.size());
	  sl::point2f p = points[c[i]];
	  in.pointlist[i*2]   =   p[0];
	  in.pointlist[i*2+1] =   p[1];
	  in.segmentlist[i*2] =   i;
	  in.segmentlist[i*2+1] = (i+1)%n;
      }
  }

  triangulate ((char*)("zp"), &in, &out, NULL);

  {
    for (int i=0; i<out.numberoftriangles; i++) {
      sl::tuple3i t;

      t = c[out.trianglelist[i*3+0]],
	  c[out.trianglelist[i*3+1]],
	  c[out.trianglelist[i*3+2]];
      result.push_back(t);
    }
  }

  delete[] in.pointlist; in.pointlist = NULL;
  delete[] in.segmentlist; in.segmentlist = NULL;
  
  free(out.trianglelist); out.trianglelist = NULL;

  return result;
}

std::pair<int, sl::point2f> bsGeometryHelpers::triangleCoordinates(const std::vector<sl::tuple3i>& triangulation,
							    const std::vector<sl::point2f>& points,
							    const sl::point2f& P) {
    std::pair<int, sl::point2f> result = std::make_pair(-1, sl::point2f(-1.,-1.));

    for (int i=0; i< (int)triangulation.size() && result.first < 0; i++) {
	assert(triangulation[i][0] >= 0 && triangulation[i][0] < (int)points.size());
	assert(triangulation[i][1] >= 0 && triangulation[i][1] < (int)points.size());
	assert(triangulation[i][2] >= 0 && triangulation[i][2] < (int)points.size());

	sl::point2f P1 = points[triangulation[i][0]];
	sl::point2f P2 = points[triangulation[i][1]];
	sl::point2f P3 = points[triangulation[i][2]];

	if (goodTriangle(P1,P2,P3)) {
	    sl::point2f uv = cartToBary(P, P1,P2,P3);
	    if (uv[0] >= 0. && uv[0] <= 1. &&
		uv[1] >= 0. && uv[1] <= 1. &&
		(1.-uv[0]-uv[1]) >= 0. && (1.-uv[0]-uv[1]) <= 1.) {
		result = std::make_pair(i,uv);
	    }
	}
    }

    return result;
}

bsGeometryHelpers::polygonIntersection bsGeometryHelpers::intersect(
			     const std::vector<int>& c,
			     const std::vector<sl::point2f>& p,
			     const sl::point2f& o,
			     const sl::point2f& e) {
  assert(goodConnectivity(c,p));

  polygonIntersection result;

#if 0
  int n=c.size();
  for (int i=0; i<n; i++) {
    sl::point2f v0 = p[c[i]];
    sl::point2f v1 = p[c[(i+1)%n]];
    float d = (e-o).dot(v1-v0);
    if (sl::abs(d)>1E-7f) {
      float t = (v0-o).dot(v1-v0)/d;
      if ( t >= 0.0f && t <= 1.0f && t < result.rayParameter()) {
	float u = (o-v0).dot(e-o)/d;
	if ( u >= 0.0f && u <= 0.5f ) {
	  result =  polygonIntersection(i,t);
	} else if ( u > 0.5f && u <= 1.0f ) {
	  result =  polygonIntersection((i+1)%n,t);
	}
      }
    }   
  }
#else
  // HACK HACK 
  {
    int i = closestPoint(c, p, o);
    float t = 0.0;
    result = polygonIntersection(i,t);
  }
#endif
#ifndef NDEBUG
  if (result.closestVertexIndex() >=0) {
    std::cerr << "O: " << o[0] << " " << o[1] << std::endl;
    std::cerr << "E: " << e[0] << " " << e[1] << std::endl;
    sl::point2f pp = o + result.rayParameter() * (e-o);
    std::cerr << "  --> I: " << pp[0] << " " << pp[1] << std::endl;
    pp = p[c[result.closestVertexIndex()]];
    std::cerr << "  --> C: " << pp[0] << " " << pp[1] << std::endl;
  }
#endif
  return result;
}

int bsGeometryHelpers::closestPoint(const std::vector<int>& c,
				    const std::vector<sl::point2f>& p,
				    const sl::point2f& P) {
  assert(goodConnectivity(c,p));

  int result = -1;
  int n=c.size();
  float min_dist = 1E30f;
  for (int i=0; i<n; i++) {
    float d = P.distance_squared_to(p[c[i]]);
    if ( d < min_dist ) {
      min_dist = d;
      result = i;
    }
  }
  return result;
}


sl::point2f bsGeometryHelpers::cartToBary(const sl::point2f& P,
				  const sl::point2f& P1, 
				  const sl::point2f& P2, 
				  const sl::point2f& P3){

    assert(goodTriangle(P1,P2,P3));

    sl::column_vector2f e32= P3-P2;
    sl::column_vector2f e31= P3-P1;
    sl::column_vector2f e3P= P3-P;

    float det = e31[0]*e32[1]-e31[1]*e32[0];
    
    sl::point2f result;
    result[0] = -(e32[0]*e3P[1]-e32[1]*e3P[0])/det;
    result[1] =  (e31[0]*e3P[1]-e31[1]*e3P[0])/det;

    return result;
}

bool bsGeometryHelpers::goodTriangle(const sl::point2f& P1,
				     const sl::point2f& P2,
				     const sl::point2f& P3) {
    sl::column_vector2f e32= P3-P2;
    sl::column_vector2f e31= P3-P1;
    float det = e31[0]*e32[1]-e31[1]*e32[0];

    return sl::abs(det) >= 1E-7f;
}

sl::point2f bsGeometryHelpers::baryToCart(const sl::point2f& uv,
				  const sl::point2f& P1, 
				  const sl::point2f& P2, 
				  const sl::point2f& P3){
    return sl::point2f(uv[0]*P1[0]+uv[1]*P2[0]+(1-uv[0]-uv[1])*P3[0],
		     uv[0]*P1[1]+uv[1]*P2[1]+(1-uv[0]-uv[1])*P3[1]);
}

sl::color3f bsGeometryHelpers::rgb2hsv(const sl::color3f& rgb){
    assert(rgb[0]>=0.0f && rgb[0]<=1.0f);
    assert(rgb[1]>=0.0f && rgb[1]<=1.0f);
    assert(rgb[2]>=0.0f && rgb[2]<=1.0f);
    sl::color3f hsv; 
    float max = rgb[rgb.imax()];
    float min = rgb[rgb.imin()];
    float delta = max-min;
    hsv[2]=max;
    if (max!=0.0) {
	hsv[1]=delta/max;
	switch (rgb.imax()) {
	case 0: 
	    hsv[0]=(rgb[1]-rgb[2])/delta;
	    break;
	case 1:
	    hsv[0]=2+(rgb[2]-rgb[0])/delta;
	    break;
	case 2:
	    hsv[0]=4+(rgb[0]-rgb[1])/delta;
	    break;
	}
	hsv[0]*=60;
	if (hsv[0] < 0.0f) hsv[0]+=360; 
    }
    return hsv;
}

sl::color3f bsGeometryHelpers::hsv2rgb(const sl::color3f& hsv){
    assert(hsv[0]>=0.0f && hsv[0]<360.0f);
    assert(hsv[1]>=0.0f && hsv[1]<=1.0f);
    assert(hsv[2]>=0.0f && hsv[2]<=1.0f);
    float h = hsv[0]/60.0f;
    float s = hsv[1];
    float v = hsv[2];
    int i = (int) std::floor(h);
    float f = h-i;
    if (!(i%2)) f = 1-f;
    float m = v*(1-s);
    float n = v*(1-s*f);
    sl::color3f rgb;
    switch (i) {
    case 6: 
    case 0: rgb = v, n, m; break; 
    case 1: rgb = n, v, m; break; 
    case 2: rgb = m, v, n; break; 
    case 3: rgb = m, n, v; break; 
    case 4: rgb = n, m, v; break; 
    case 5: rgb = v, m, n; break; 
    }
    return rgb;
}

sl::color3f bsGeometryHelpers::createColor(int i) {
    sl::color3f c;
    int rot = i/6;
    float val = pow(1.1f,rot);
    switch (i%6) {
    case 0: c =   40.0f, 236.0f, 246.0f; break; // cyan
    case 1: c =  236.0f,  40.0f,  50.0f; break; // red
    case 2: c =   50.0f,  40.0f, 236.0f; break; // blue
    case 3: c =  200.0f, 144.0f,  40.0f; break; // orange
    case 4: c =   40.0f, 236.0f,  50.0f; break; // green
    case 5: c =  236.0f,  40.0f, 246.0f; break; // magenta
    }    
    c[0] = c[0]/(255.0f*val);
    c[1] = c[1]/(255.0f*val);
    c[2] = c[2]/(255.0f*val);
    return c;
}

float bsGeometryHelpers::luminance(const sl::color3f& rgb){
  return 0.29900f*rgb[0]+0.58600f*rgb[1]+0.11400f*rgb[2]; 
}


// added (sergio 20.02.2001)
sl::point2f bsGeometryHelpers::findInternalPoint(const std::vector<int>& c,
					       const std::vector<sl::point2f>& p) {

  assert(goodConnectivity(c, p));

  sl::point2f result;
  sl::column_vector2f v0, v1, v2;  
  bool found = false;

  for (int i=0; i<(int)c.size()-2 && !found; i++) {
    v0 = p[c[i+0]]-sl::point2f::zero();
    v1 = p[c[i+1]]-sl::point2f::zero();
    v2 = p[c[i+2]]-sl::point2f::zero();

    result = sl::point2f::zero() + (v0+v1+v2)/3.0f;    
    // find center of mass of the triangle

    if (isInside(c, p, result)) {
      found = 1;
    }
  }
  assert(found);

  return result;
} 

