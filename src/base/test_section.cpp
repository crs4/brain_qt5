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
#include "bsInsideIO.h"
#include <iostream>

int main() {

  std::cerr << "Starting test\n";

  std::vector< sl::point2f >    points;
  std::vector< int >          external_boundary_connectivity;
  std::vector< int >          internal_boundary_connectivity;
  std::vector< std::vector<int> >  holes;
  std::vector< sl::tuple2i >    cuts;
  std::vector< int >          region_ids;

  sl::point2f p0; p0 = -2.0f,-2.0f; 
  sl::point2f p1; p1 =  2.0f,-2.0f;
  sl::point2f p2; p2 =  2.0f, 2.0f;
  sl::point2f p3; p3 = -2.0f, 2.0f;

  sl::point2f p4; p4 = -1.0f,-1.0f; 
  sl::point2f p5; p5 =  1.0f,-1.0f;
  sl::point2f p6; p6 =  1.0f, 1.0f;
  sl::point2f p7; p7 = -1.0f, 1.0f;

  points.push_back(p0);
  points.push_back(p1);
  points.push_back(p2);
  points.push_back(p3);  

  points.push_back(p4);
  points.push_back(p5);
  points.push_back(p6);
  points.push_back(p7);
  
  external_boundary_connectivity.push_back(0);
  external_boundary_connectivity.push_back(1);
  external_boundary_connectivity.push_back(2);
  external_boundary_connectivity.push_back(3);
  
  internal_boundary_connectivity.push_back(4);
  internal_boundary_connectivity.push_back(5);
  internal_boundary_connectivity.push_back(6);
  internal_boundary_connectivity.push_back(7);

  sl::tuple2i tup; 
  tup = 0,0; cuts.push_back(tup);
  tup = 1,1; cuts.push_back(tup);

  region_ids.push_back(1);
  region_ids.push_back(0);

  bsSection* s = new bsSection(points,
			       external_boundary_connectivity,
			       internal_boundary_connectivity,
			       holes,
			       cuts,
			       region_ids);
  
  s->setQuota(2.);

  sl::point2f n0; n0 = 0.0f,-1.5f; 
  sl::point2f n1; n1 = 1.5f, 1.5f; 

  s->appendNeuron(0,n0);
  s->appendNeuron(1,n1);

  s->assignNeurons();

  std::cout << *s << std::endl;

}

