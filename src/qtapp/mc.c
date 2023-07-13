/* mc.c Marching Cubes implementation for lab1
   Mike Chupa
   CS 8413: Visualization
   Instructor: Dr. Raghu Machiraju
   Copyright (c) 1998 Michael A. Chupa Permission is expressly granted
   to use this code in any non-commercial work, provided that this
   notice is preserved. However, be aware that the Marching Cubes
   algorithm is patented.
   */

#include "mc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define EPSILON 0.000001

/* tag for an empty edge */
#define EMPTY_EDGE -1


static long makeVertex(int interpolate, 
 		       int whichEdge, 
		       int iDim, int jDim, int kDim, 
		       int i, int j, int k,
		       float threshold, 
		       mc_datum *dataset, 
		       mc_isosurface *layerIso);

/*
   creates a vertex and normal for the isosurface in the cell
   (i, j, k) and edge specified, using threshold to interpolate
   the vertex position, and the gradient of the scalar field to
   compute the normal. Return value is the index into the
   vertex/normal list for the indexed face set Inventor node.
*/
   
/*
  these tables for computing the Marching Cubes algorithm
  are from http://www.mhri.edu.au/~pdb/modelling/polygonise/
  by Paul Bourke, based on code by Cory Gene Bloyd.

  The indexing of vertices and edges in a cube are defined
  as: 
  
                _4____________4_____________5
               /|                           /
              / |                          /|
             /  |                         / |
            7   |                        /  |
           /    |                       /5  |
          /     |                      /    |
         /      8                     /     9
	/       |                    /      |
      7/________|______6____________/6      |
       |        |                   |       |
       |        |                   |       |
       |        |                   |       |
       |        |0____________0_____|_______|1
      11       /                    |      /
       |      /                    10     /
       |     /                      |    /
       |    /3                      |   /1
       |   /                        |  /
       |  /                         | /
       | /                          |/
       |/3____________2_____________|2

 


For purposes of calculating vertices along the edges and the
triangulations created, there are 15 distinct cases, with
upper limits of
  12 edge intersections
  5 triangles created per cell
*/

int edgeTable[256]={
  0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
  0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
  0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
  0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
  0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
  0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
  0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
  0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
  0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
  0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
  0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
  0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
  0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
  0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
  0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
  0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
  0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
  0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
  0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
  0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
  0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
  0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
  0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
  0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
  0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
  0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
  0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
  0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
  0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
  0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
  0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
  0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };


int triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
 {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
 {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
 {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
 {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
 {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
 {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
 {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
 {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
 {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
 {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
 {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
 {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
 {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
 {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
 {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
 {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
 {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
 {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
 {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
 {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
 {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
 {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
 {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
 {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
 {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
 {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
 {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
 {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
 {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
 {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
 {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
 {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
 {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
 {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
 {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
 {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
 {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
 {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
 {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
 {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
 {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
 {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
 {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
 {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
 {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
 {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
 {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
 {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
 {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
 {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
 {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
 {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
 {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
 {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
 {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
 {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
 {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
 {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
 {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
 {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
 {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
 {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
 {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
 {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
 {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
 {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
 {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
 {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
 {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
 {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
 {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
 {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
 {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
 {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
 {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
 {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
 {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
 {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
 {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
 {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
 {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
 {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
 {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
 {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
 {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
 {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
 {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
 {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
 {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
 {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
 {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
 {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
 {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
 {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
 {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
 {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
 {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
 {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
 {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
 {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
 {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
 {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
 {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
 {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
 {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
 {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
 {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
 {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
 {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
 {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
 {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
 {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
 {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
 {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
 {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
 {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
 {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
 {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
 {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
 {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
 {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
 {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
 {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
 {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
 {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
 {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
 {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
 {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
 {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
 {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
 {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
 {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
 {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
 {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
 {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
 {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
 {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
 {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
 {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
 {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
 {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
 {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
 {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
 {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
 {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
 {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
 {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
 {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
 {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
 {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
 {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
 {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
 {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
 {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
 {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
 {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
 {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
 {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
 {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
 {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
 {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
 {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
 {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
 {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
 {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
 {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
 {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
 {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
 {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
 {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
 {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
 {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
 {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
 {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
 {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
 {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
 {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
 {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
 {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};


void mc_iso_free(mc_isosurface * iso) {
    int i;
    for (i = 0; i < 3; i++) {
	free(iso->vertices[i]);
	free(iso->normals[i]);
	free(iso->triangles[i]);
    }
    free(iso);
}


void mc_iso_append(mc_isosurface* iso1, mc_isosurface* iso2) {

  int i, j;

  {
    for (i=0; i<3; i++) {

      iso1->vertices[i]  = (float *) realloc(iso1->vertices[i], 
					     sizeof(float) * (iso1->nVertices+iso2->nVertices));
      iso1->normals[i]   = (float *) realloc(iso1->normals[i], 
      	  			             sizeof(float) * (iso1->nVertices+iso2->nVertices));
      iso1->triangles[i] = (long *)  realloc(iso1->triangles[i], 
					     sizeof(long) * (iso1->nTriangles+iso2->nTriangles));
      
    }
  }
  {
    for (i=0; i<3; i++) {
      for (j=0; j<iso2->nVertices; j++) {
	iso1->vertices[i][iso1->nVertices+j] = iso2->vertices[i][j];
	iso1->normals[i][iso1->nVertices+j] = iso2->normals[i][j];
      }
    }
  }
  {
    for (i=0; i<3; i++) {
      for (j=0; j<iso2->nTriangles; j++) {
	iso1->triangles[i][iso1->nTriangles+j] = iso1->nVertices+iso2->triangles[i][j];
      }
    }
  }

  iso1->nVertices  += iso2->nVertices;
  iso1->nTriangles += iso2->nTriangles;
} 


mc_isosurface *mc_march_disconnected_layer(mc_datum *dataset, 
					   int iDim, 
					   int jDim, 
					   int kDim, 
					   int k, 
					   float threshold,
					   int interpolate) {
    
  mc_isosurface *iso = (mc_isosurface *) malloc(sizeof(mc_isosurface));
  mc_layer *layer = (mc_layer *) malloc(sizeof(mc_layer));

  {
      int i;
      for (i = 0; i < 3; i++) {
	  iso->vertices[i]  = NULL;
	  iso->normals[i]   = NULL;
	  iso->triangles[i] = NULL;
      }
      iso->nVertices    = 0;
      iso->nTriangles   = 0;
  }

  layer->botData = dataset + MC_OFFSET(0, 0, k,   iDim, jDim);
  layer->topData = dataset + MC_OFFSET(0, 0, k+1, iDim, jDim);
  layer->edges = (long *) malloc((iDim-1) * (jDim-1) * 12 * sizeof(long));

  /* initialize the edge table to EMPTY */
  {
      int i;
      for (i=0; i<(iDim-1)*(jDim-1)*12; i++) {
	  *(layer->edges+i) = EMPTY_EDGE;
      }
  }

  mc_march_layer(dataset, layer, iDim, jDim, kDim, k, threshold, interpolate, iso);

  free(layer->edges);
  free(layer);

  return iso;
}

mc_isosurface *mc_march(mc_datum *dataset, 
			int iDim, 
			int jDim, 
			int kDim, 
			float threshold,
			int interpolate) {
  int k, i ,j, e;
    
  mc_isosurface *iso = (mc_isosurface *) malloc(sizeof(mc_isosurface));
  mc_layer *layer = (mc_layer *) malloc(sizeof(mc_layer));

  {
      int i;
      for (i = 0; i < 3; i++) {
	  iso->vertices[i]  = NULL;
	  iso->normals[i]   = NULL;
	  iso->triangles[i] = NULL;
      }
      iso->nVertices    = 0;
      iso->nTriangles   = 0;
  }

  layer->botData = dataset + MC_OFFSET(0, 0, 0,   iDim, jDim);
  layer->topData = dataset + MC_OFFSET(0, 0, 1, iDim, jDim);
  layer->edges = (long *) malloc((iDim-1) * (jDim-1) * 12 * sizeof(long));

  /* initialize the edge table to EMPTY */
  {
      int i;
      for (i=0; i<(iDim-1)*(jDim-1)*12; i++) {
	  *(layer->edges+i) = EMPTY_EDGE;
      }
  }

  mc_march_layer(dataset, layer, iDim, jDim, kDim, 0, threshold, interpolate, iso);

  for (k = 1; k < kDim - 1; k++) {
    /* update the layer for this iteration */
    layer->botData = layer->topData;
    /* now topData points to next layer of scalar data*/
    layer->topData = dataset + MC_OFFSET(0, 0, k+1, iDim, jDim);

    /* percolate the last layer's top edges to this layer's bottom edges */
    for (i = 0; i < iDim-1; i++) {
      for (j = 1; j < jDim; j++) {
	for (e = 0; e < 4; e++) {
	  *(layer->edges + MC_EDGE_OFFSET(e, i, j, iDim)) = 
	    *(layer->edges + MC_EDGE_OFFSET(e+4, i, j, iDim));
	}
	/* reinitialize all of the remaining edges */
	for (e = 4; e < 12; e++) {
	  *(layer->edges + MC_EDGE_OFFSET(e, i, j, iDim)) = EMPTY_EDGE;
	}
      }
    }
    mc_march_layer(dataset, layer, iDim, jDim, kDim, k, threshold, interpolate, iso);
  }

  free(layer->edges);
  free(layer);

  return iso;
}



void mc_march_layer(mc_datum *dataset, mc_layer *layer, int iDim, int jDim, int kDim, 
	      int k, float threshold, int interpolate, mc_isosurface *iso) {

  mc_isosurface    *layerIso;          /* portion of isosurface in layer   */

  float         *fPtr;
  long          *lPtr;
  mc_datum          cell[8];           /* grid values at 8 cell vertices   */
  long           cellVerts[12];     /* verts at the 12 edges in a cell  */
  int            i, j, ii, jj, e;
  long           vertSize;          /* allocation for verts and normals */
  long           triSize;           /* allocation for triangles         */
  unsigned char  cellIndex;         /* index for edgeTable lookup       */



  /* initialize the cellVerts table */
  for (i = 0; i < 12; i++) {
    cellVerts[i] = EMPTY_EDGE;
  }

  /* allocate and initialize storage for this layer's portion of isosurface */
  layerIso = (mc_isosurface *) malloc(sizeof(mc_isosurface));

  /* we have a maximum of 12 verts and 5 triangles per cell*/
  vertSize = (iDim - 1) * (jDim - 1) * 12 * sizeof(float);
  triSize  = (iDim - 1) * (jDim - 1) * 5  * sizeof(int);
  /* allocate storage for vertices, normals and triangles */
  for (i = 0; i < 3; i++) {
    layerIso->vertices[i]  = (float *) malloc(vertSize);
    layerIso->normals[i]   = (float *) malloc(vertSize);
    layerIso->triangles[i] = (long  *) malloc(triSize );
  
    if ((layerIso->vertices[i]  == NULL) || 
	(layerIso->normals[i]   == NULL) || 
	(layerIso->triangles[i] == NULL)) {
      fprintf(stderr, "march: failed to allocate layer isosurface\n");
      exit(1);
    }
  }
  /* init counts */
  layerIso->nVertices = 0;
  layerIso->nTriangles = 0;

  /* for each cell in the grid... */
  for(i = 0; i < iDim - 1; i++) {
    for(j = 1; j < jDim; j++) {
      /* do the marching cubes thing */
      /* fill the cell */
      cell[0] = *(layer->botData +   j   * iDim +   i);
      cell[1] = *(layer->botData +   j   * iDim + i + 1);
      cell[2] = *(layer->botData + (j-1) * iDim + i + 1);
      cell[3] = *(layer->botData + (j-1) * iDim +   i);
      cell[4] = *(layer->topData +   j   * iDim +   i);
      cell[5] = *(layer->topData +   j   * iDim + i + 1);
      cell[6] = *(layer->topData + (j-1) * iDim + i + 1);
      cell[7] = *(layer->topData + (j-1) * iDim +   i);

      /* compute the index for the edge intersections */
      cellIndex = 0;
      if (cell[0] < threshold) cellIndex |=   1;
      if (cell[1] < threshold) cellIndex |=   2;
      if (cell[2] < threshold) cellIndex |=   4;
      if (cell[3] < threshold) cellIndex |=   8;
      if (cell[4] < threshold) cellIndex |=  16;
      if (cell[5] < threshold) cellIndex |=  32;
      if (cell[6] < threshold) cellIndex |=  64;
      if (cell[7] < threshold) cellIndex |= 128;     

      /* get the coordinates for the vertices */
      /* compute the triangulation */      
      /* interpolate the normals */
      if (edgeTable[cellIndex] &    1) {
	if (*(layer->edges + MC_EDGE_OFFSET(0, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[0]  = iso->nVertices +
	                  makeVertex(interpolate, 0, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[0] = *(layer->edges + MC_EDGE_OFFSET(0, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    2) {
	if (*(layer->edges + MC_EDGE_OFFSET(1, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[1]  = iso->nVertices +
	                  makeVertex(interpolate, 1, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[1] = *(layer->edges + MC_EDGE_OFFSET(1, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    4) {
	if (*(layer->edges + MC_EDGE_OFFSET(2, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[2]  = iso->nVertices +
	                  makeVertex(interpolate, 2, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[2] = *(layer->edges + MC_EDGE_OFFSET(2, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    8) {
	if (*(layer->edges + MC_EDGE_OFFSET(3, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[3]  = iso->nVertices +
	                  makeVertex(interpolate, 3, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[3] = *(layer->edges + MC_EDGE_OFFSET(3, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    16) {
	if (*(layer->edges + MC_EDGE_OFFSET(4, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[4]  = iso->nVertices +
	                  makeVertex(interpolate, 4, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[4] = *(layer->edges + MC_EDGE_OFFSET(4, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    32) {
	if (*(layer->edges + MC_EDGE_OFFSET(5, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[5]  = iso->nVertices +
	                  makeVertex(interpolate, 5, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[5] = *(layer->edges + MC_EDGE_OFFSET(5, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    64) {
	if (*(layer->edges + MC_EDGE_OFFSET(6, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[6]  = iso->nVertices +
	                  makeVertex(interpolate, 6, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[6] = *(layer->edges + MC_EDGE_OFFSET(6, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    128) {
	if (*(layer->edges + MC_EDGE_OFFSET(7, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[7]  = iso->nVertices +
	                  makeVertex(interpolate, 7, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[7] = *(layer->edges + MC_EDGE_OFFSET(7, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    256) {
	if (*(layer->edges + MC_EDGE_OFFSET(8, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[8]  = iso->nVertices +
	                  makeVertex(interpolate, 8, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[8] = *(layer->edges + MC_EDGE_OFFSET(8, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    512) {
	if (*(layer->edges + MC_EDGE_OFFSET(9, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[9]  = iso->nVertices +
	                  makeVertex(interpolate, 9, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[9] = *(layer->edges + MC_EDGE_OFFSET(9, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    1024) {
	if (*(layer->edges + MC_EDGE_OFFSET(10, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[10]  = iso->nVertices +
	                  makeVertex(interpolate, 10, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[10] = *(layer->edges + MC_EDGE_OFFSET(10, i, j, iDim));
	}
      }
      if (edgeTable[cellIndex] &    2048) {
	if (*(layer->edges + MC_EDGE_OFFSET(11, i, j, iDim)) == EMPTY_EDGE) {     
	  cellVerts[11]  = iso->nVertices +
	                  makeVertex(interpolate, 11, iDim, jDim, kDim, 
				     i, j, k, threshold, dataset, layerIso);
	}
	else {
	  cellVerts[11] = *(layer->edges + MC_EDGE_OFFSET(11, i, j, iDim));
	}
      }

      /* put the cellVerts tags into this cell's layer->edges table */
      for (e = 0; e < 12; e++) {
	if (cellVerts[e] != EMPTY_EDGE) {
	  *(layer->edges + MC_EDGE_OFFSET(e, i, j, iDim)) = cellVerts[e];
	}
      }

      /* now propagate the vertex/normal tags to the adjacent cells to */
      /* the right and behind in this layer. */
      if (i < iDim - 2) { /* we should propagate to the right */
	*(layer->edges + MC_EDGE_OFFSET( 3, i+1, j, iDim)) = cellVerts[1];
	*(layer->edges + MC_EDGE_OFFSET( 7, i+1, j, iDim)) = cellVerts[5];
	*(layer->edges + MC_EDGE_OFFSET( 8, i+1, j, iDim)) = cellVerts[9];
	*(layer->edges + MC_EDGE_OFFSET(11, i+1, j, iDim)) = cellVerts[10];
      }
      if (j < jDim - 1) { /* we should propagate to the rear */
	*(layer->edges + MC_EDGE_OFFSET( 2, i, j+1, iDim)) = cellVerts[0];
	*(layer->edges + MC_EDGE_OFFSET( 6, i, j+1, iDim)) = cellVerts[4];
	*(layer->edges + MC_EDGE_OFFSET(11, i, j+1, iDim)) = cellVerts[8];
	*(layer->edges + MC_EDGE_OFFSET(10, i, j+1, iDim)) = cellVerts[9];      
      }      
      

      /* compute the triangulation */      
      ii = 0;
      while (triTable[cellIndex][ii] != -1) {
	for (jj = 0; jj < 3; jj++) {
	  *(layerIso->triangles[jj] + layerIso->nTriangles) = cellVerts[triTable[cellIndex][ii++]];
	}
	layerIso->nTriangles++;
      }
    
    }
  }  
  
  /* now that we have the new verts, normals and triangles */
  /* calculated, we need to add them to the master lists   */

#ifndef NDEBUG
  fprintf(stderr, "%5ld verts, %5ld triangles\n", layerIso->nVertices, layerIso->nTriangles);
#endif
  
  /* if new verts, expand the storage for the master lists */
  if (layerIso->nVertices > 0) {
    for(i = 0; i < 3; i++) {
      iso->vertices[i]   = (float *) realloc((void *) iso->vertices[i],
					     (iso->nVertices + layerIso->nVertices) * sizeof(float));
      iso->normals[i]    = (float *) realloc((void *) iso->normals[i],
					     (iso->nVertices + layerIso->nVertices) * sizeof(float));
      iso->triangles[i]  = (long  *) realloc((void *) iso->triangles[i],
					     (iso->nTriangles + layerIso->nTriangles) * sizeof(long));

      if ((iso->vertices[i] == NULL) ||
	  (iso->normals[i]  == NULL) ||
	  (iso->triangles[i] == NULL)) {
	fprintf(stderr, "realloc failed\n");
	exit(1);
      }

      /* tack on the new data to the master lists */
      fPtr = iso->vertices[i] + iso->nVertices;
      memcpy((void *) fPtr, layerIso->vertices[i], layerIso->nVertices * sizeof(float));
      fPtr = iso->normals[i] + iso->nVertices;
      memcpy((void *) fPtr, layerIso->normals[i], layerIso->nVertices * sizeof(float));
      lPtr = iso->triangles[i] + iso->nTriangles;
      memcpy((void *) lPtr, layerIso->triangles[i], layerIso->nTriangles * sizeof(long));
      
    }
    /* update the master list counts */
    iso->nVertices  += layerIso->nVertices;
    iso->nTriangles += layerIso->nTriangles;

#ifndef NDEBUG
    fprintf(stderr, "\t\ttotals: %ld verts and %ld triangles\n",
	    iso->nVertices, iso->nTriangles);
#endif

  }

  /* toss the storage for this run of march */ 
  for (i = 0; i < 3; i++) {
    free(layerIso->vertices[i]);
    free(layerIso->normals[i]);
    free(layerIso->triangles[i]);
  }
  free(layerIso);
}


static long makeVertex(int interpolate, 
		       int whichEdge, 
		       int iDim, int jDim, int kDim, 
		       int i, int j, int k,
		       float threshold, mc_datum *dataset, mc_isosurface *layerIso) {

  int       from[3]; /* first grid vertex                              */
  int         to[3]; /* second grid vertex                             */
  float normFrom[3]; /* normal at from vertex                          */
  float   normTo[3]; /* normal at the to vertex                        */
  float        v[3]; /* the interpolated vertex                        */
  float        n[3]; /* and its normal                                 */
  float        d;    /* relative distance along edge for interpolation */
  float      len;    /* normal length for normalization                */
  int         ii;

  switch (whichEdge) {
  case 0: 
    from[0] =  i;  from[1] =  j;  from[2] =  k;
      to[0] = i+1;   to[1] =  j;    to[2] =  k;
    break;
  case 1: 
    from[0] = i+1; from[1] =  j;  from[2] =  k;
      to[0] = i+1;   to[1] = j-1;   to[2] =  k;
    break;
  case 2: 
    from[0] = i+1; from[1] = j-1; from[2] =  k;
      to[0] =  i;    to[1] = j-1;   to[2] =  k;
    break;
  case 3: 
    from[0] =  i;  from[1] = j-1; from[2] =  k;
      to[0] =  i;    to[1] =  j;    to[2] =  k;
    break;
  case 4: 
    from[0] =  i;  from[1] =  j;  from[2] = k+1;
      to[0] = i+1;   to[1] =  j;    to[2] = k+1;
    break;
  case 5: 
    from[0] = i+1; from[1] =  j;  from[2] = k+1;
      to[0] = i+1;   to[1] = j-1;   to[2] = k+1;
    break;
  case 6: 
    from[0] = i+1; from[1] = j-1; from[2] = k+1;
      to[0] =  i;    to[1] = j-1;   to[2] = k+1;
    break;
  case 7: 
    from[0] =  i;  from[1] = j-1; from[2] = k+1;
      to[0] =  i;    to[1] =  j;    to[2] = k+1;
    break;
  case 8: 
    from[0] =  i;  from[1] =  j;  from[2] =  k;
      to[0] =  i;    to[1] =  j;    to[2] = k+1;
    break;
  case 9: 
    from[0] = i+1; from[1] =  j;  from[2] =  k;
      to[0] = i+1;   to[1] =  j;    to[2] = k+1;
    break;
  case 10: 
    from[0] = i+1; from[1] = j-1; from[2] =  k;
      to[0] = i+1;   to[1] = j-1;   to[2] = k+1;
    break;
  case 11: 
    from[0] =  i;  from[1] = j-1; from[2] =  k;
      to[0] =  i;    to[1] = j-1;   to[2] = k+1;
    break;
  default:
    fprintf(stderr, "mc: makeVertex: bad edge index\n");
    exit(1);
    break;
  }

  if (!interpolate) {
      /*****************/
      mc_datum d_from = *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim));
      mc_datum d_to   = *(dataset + MC_OFFSET(to[0], to[1], to[2], iDim, jDim));

      int vi[3];

      if (d_from > d_to) {
	  vi[0] = from[0];
	  vi[1] = from[1];
	  vi[2] = from[2];
      } else {
	  vi[0] = to[0];
	  vi[1] = to[1];
	  vi[2] = to[2];
      }
      
      v[0] = (float)vi[0];
      v[1] = (float)vi[1];
      v[2] = (float)vi[2];

      if (vi[0] == 0) { /* on left edge */
	  n[0] = 0.5 * (-3.0 * *(dataset + MC_OFFSET(vi[0], vi[1], vi[2], iDim, jDim)) +
			4.0 * *(dataset + MC_OFFSET(vi[0]+1, vi[1], vi[2], iDim, jDim)) -
			*(dataset + MC_OFFSET(vi[0]+2, vi[1], vi[2], iDim, jDim)));
      } else if (vi[0] == iDim - 1) { /* on right edge */
	  n[0] = 0.5 * (       *(dataset + MC_OFFSET(vi[0]-2, vi[1], vi[2], iDim, jDim)) -
			       4.0 * *(dataset + MC_OFFSET(vi[0]-1, vi[1], vi[2], iDim, jDim)) +
			       3.0 * *(dataset + MC_OFFSET(vi[0], vi[1], vi[2], iDim, jDim)));
      } else { /* in the interior */
	  n[0] = 0.5 * ( *(dataset + MC_OFFSET(vi[0]+1, vi[1], vi[2], iDim, jDim)) -
			 *(dataset + MC_OFFSET(vi[0]-1, vi[1], vi[2], iDim, jDim)));
      }
      
      if (vi[1] == 0) { /* on front edge */
	  n[1] = 0.5 * (-3.0 * *(dataset + MC_OFFSET(vi[0], vi[1], vi[2], iDim, jDim)) +
			4.0 * *(dataset + MC_OFFSET(vi[0], vi[1]+1, vi[2], iDim, jDim)) -
			*(dataset + MC_OFFSET(vi[0], vi[1]+2, vi[2], iDim, jDim)));
      } else if (vi[1] == jDim - 1) { /* on back edge */
	  n[1] = 0.5 * (       *(dataset + MC_OFFSET(vi[0], vi[1]-2, vi[2], iDim, jDim)) -
			       4.0 * *(dataset + MC_OFFSET(vi[0], vi[1]-1, vi[2], iDim, jDim)) +
			       3.0 * *(dataset + MC_OFFSET(vi[0], vi[1], vi[2], iDim, jDim)));
      } else { /* in the interior */
	  n[1] = 0.5 * ( *(dataset + MC_OFFSET(vi[0], vi[1]+1, vi[2], iDim, jDim)) -
			 *(dataset + MC_OFFSET(vi[0], vi[1]-1, vi[2], iDim, jDim)));
      }
      
      if (vi[2] == 0) { /* on bottom edge */
	  n[2] = 0.5 * (-3.0 * *(dataset + MC_OFFSET(vi[0], vi[1], vi[2], iDim, jDim)) +
			4.0 * *(dataset + MC_OFFSET(vi[0], vi[1], vi[2]+1, iDim, jDim)) -
			*(dataset + MC_OFFSET(vi[0], vi[1], vi[2]+2, iDim, jDim)));
      } else if (vi[2] == kDim - 1) { /* on top edge */
	  n[2] = 0.5 * (       *(dataset + MC_OFFSET(vi[0], vi[1], vi[2]-2, iDim, jDim)) -
			       4.0 * *(dataset + MC_OFFSET(vi[0], vi[1], vi[2]-1, iDim, jDim)) +
			       3.0 * *(dataset + MC_OFFSET(vi[0], vi[1], vi[2], iDim, jDim)));
      } else { /* in the interior */
	  n[2] = 0.5 * ( *(dataset + MC_OFFSET(vi[0], vi[1], vi[2]+1, iDim, jDim)) -
			 *(dataset + MC_OFFSET(vi[0], vi[1], vi[2]-1, iDim, jDim)));
      }

  } else {
      /*******************/

      /* determine the relative distance along edge from->to */
      /* that the isosurface vertex lies */
      d = ( *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim)) - threshold) / 
	  ( *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim)) - 
	    *(dataset + MC_OFFSET(to[0], to[1], to[2], iDim, jDim)) );
      if (d < EPSILON) {
	  d = 0.0;
      }
      else if (d > (1 - EPSILON)) {
	  d = 1.0;
      }

      v[0] = from[0] + d * (to[0] - from[0]);
      v[1] = from[1] + d * (to[1] - from[1]);
      v[2] = from[2] + d * (to[2] - from[2]);
      
      /* now determine the gradients at the endpoints of the edge */
      /* and interpolate the normal for the isosurface vertex     */

      /* the gradients are computed by central differences, except */
      /* on the boundaries of the dataset, where forward or backward */
      /* differencing is used (three point form) */
      if (from[0] == 0) { /* on left edge */
	  normFrom[0] = 0.5 * (-3.0 * *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim)) +
			       4.0 * *(dataset + MC_OFFSET(from[0]+1, from[1], from[2], iDim, jDim)) -
			       *(dataset + MC_OFFSET(from[0]+2, from[1], from[2], iDim, jDim)));
      }
      else if (from[0] == iDim - 1) { /* on right edge */
	  normFrom[0] = 0.5 * (       *(dataset + MC_OFFSET(from[0]-2, from[1], from[2], iDim, jDim)) -
				      4.0 * *(dataset + MC_OFFSET(from[0]-1, from[1], from[2], iDim, jDim)) +
				      3.0 * *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim)));
      }
      else { /* in the interior */
	  normFrom[0] = 0.5 * ( *(dataset + MC_OFFSET(from[0]+1, from[1], from[2], iDim, jDim)) -
				*(dataset + MC_OFFSET(from[0]-1, from[1], from[2], iDim, jDim)));
      }
      
      if (from[1] == 0) { /* on front edge */
	  normFrom[1] = 0.5 * (-3.0 * *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim)) +
			  4.0 * *(dataset + MC_OFFSET(from[0], from[1]+1, from[2], iDim, jDim)) -
			       *(dataset + MC_OFFSET(from[0], from[1]+2, from[2], iDim, jDim)));
      }
      else if (from[1] == jDim - 1) { /* on back edge */
	  normFrom[1] = 0.5 * (       *(dataset + MC_OFFSET(from[0], from[1]-2, from[2], iDim, jDim)) -
				      4.0 * *(dataset + MC_OFFSET(from[0], from[1]-1, from[2], iDim, jDim)) +
				      3.0 * *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim)));
      }
      else { /* in the interior */
	  normFrom[1] = 0.5 * ( *(dataset + MC_OFFSET(from[0], from[1]+1, from[2], iDim, jDim)) -
				*(dataset + MC_OFFSET(from[0], from[1]-1, from[2], iDim, jDim)));
      }
    
      if (from[2] == 0) { /* on bottom edge */
	  normFrom[2] = 0.5 * (-3.0 * *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim)) +
			       4.0 * *(dataset + MC_OFFSET(from[0], from[1], from[2]+1, iDim, jDim)) -
			       *(dataset + MC_OFFSET(from[0], from[1], from[2]+2, iDim, jDim)));
      }
      else if (from[2] == kDim - 1) { /* on top edge */
	  normFrom[2] = 0.5 * (       *(dataset + MC_OFFSET(from[0], from[1], from[2]-2, iDim, jDim)) -
				      4.0 * *(dataset + MC_OFFSET(from[0], from[1], from[2]-1, iDim, jDim)) +
				      3.0 * *(dataset + MC_OFFSET(from[0], from[1], from[2], iDim, jDim)));
      }
      else { /* in the interior */
	  normFrom[2] = 0.5 * ( *(dataset + MC_OFFSET(from[0], from[1], from[2]+1, iDim, jDim)) -
				*(dataset + MC_OFFSET(from[0], from[1], from[2]-1, iDim, jDim)));
      }
      
      
      /* normal for to vertex */
      
      if (to[0] == 0) { /* on left edge */
	  normTo[0] = 0.5 * (  -3.0 * *(dataset + MC_OFFSET(to[0], to[1], to[2], iDim, jDim)) +
			       4.0 * *(dataset + MC_OFFSET(to[0]+1, to[1], to[2], iDim, jDim)) -
			       *(dataset + MC_OFFSET(to[0]+2, to[1], to[2], iDim, jDim)));
      }
      else if (to[0] == iDim - 1) { /* on right edge */
	  normTo[0] = 0.5 * (         *(dataset + MC_OFFSET(to[0]-2, to[1], to[2], iDim, jDim)) -
				      4.0 * *(dataset + MC_OFFSET(to[0]-1, to[1], to[2], iDim, jDim)) +
				      3.0 * *(dataset + MC_OFFSET(to[0], to[1], to[2], iDim, jDim)));
      }
      else { /* in the interior */
	  normTo[0] = 0.5 * ( *(dataset + MC_OFFSET(to[0]+1, to[1], to[2], iDim, jDim)) -
			      *(dataset + MC_OFFSET(to[0]-1, to[1], to[2], iDim, jDim)));
      }
      
      if (to[1] == 0) { /* on front edge */
	  normTo[1] = 0.5 * (  -3.0 * *(dataset + MC_OFFSET(to[0], to[1], to[2], iDim, jDim)) +
			       4.0 * *(dataset + MC_OFFSET(to[0], to[1]+1, to[2], iDim, jDim)) -
			       *(dataset + MC_OFFSET(to[0], to[1]+2, to[2], iDim, jDim)));
      }
      else if (to[1] == jDim - 1) { /* on back edge */
	  normTo[1] = 0.5 * (         *(dataset + MC_OFFSET(to[0], to[1]-2, to[2], iDim, jDim)) -
				      4.0 * *(dataset + MC_OFFSET(to[0], to[1]-1, to[2], iDim, jDim)) +
				      3.0 * *(dataset + MC_OFFSET(to[0], to[1], to[2], iDim, jDim)));
      }
      else { /* in the interior */
	  normTo[1] = 0.5 * ( *(dataset + MC_OFFSET(to[0], to[1]+1, to[2], iDim, jDim)) -
			      *(dataset + MC_OFFSET(to[0], to[1]-1, to[2], iDim, jDim)));
      }
      
      if (to[2] == 0) { /* on bottom edge */
	  normTo[2] = 0.5 * (  -3.0 * *(dataset + MC_OFFSET(to[0], to[1], to[2], iDim, jDim)) +
			       4.0 * *(dataset + MC_OFFSET(to[0], to[1], to[2]+1, iDim, jDim)) -
			       *(dataset + MC_OFFSET(to[0], to[1], to[2]+2, iDim, jDim)));
      }
      else if (to[2] == kDim - 1) { /* on top edge */
	  normTo[2] = 0.5 * (         *(dataset + MC_OFFSET(to[0], to[1], to[2]-2, iDim, jDim)) -
				      4.0 * *(dataset + MC_OFFSET(to[0], to[1], to[2]-1, iDim, jDim)) +
				      3.0 * *(dataset + MC_OFFSET(to[0], to[1], to[2], iDim, jDim)));
      }
      else { /* in the interior */
	  normTo[2] = 0.5 * ( *(dataset + MC_OFFSET(to[0], to[1], to[2]+1, iDim, jDim)) -
			      *(dataset + MC_OFFSET(to[0], to[1], to[2]-1, iDim, jDim)));
      }

     
      /* now that we have the normals at the two endpoints, interpolate */
    
      n[0] = normFrom[0] + d * (normTo[0] - normFrom[0]);
      n[1] = normFrom[1] + d * (normTo[1] - normFrom[1]);
      n[2] = normFrom[2] + d * (normTo[2] - normFrom[2]);
  } 

  /* normalize the normal at the isosurface vertex */
  len = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
  if (len > EPSILON) {
    n[0] /= len;
    n[1] /= len;
    n[2] /= len;
  }
  else { /* we have to fake this normal */
    n[0] = 1.0;
    n[1] = 0.0;
    n[2] = 0.0;
  }
  
  /* insert the vertex and normal into the isosurface structure */
  /* compute the tag for this vertex, and return the tag        */
  
  for (ii = 0; ii < 3; ii++) {
    *(layerIso->vertices[ii] + layerIso->nVertices) = v[ii];
    *(layerIso->normals[ii]  + layerIso->nVertices) = n[ii];
    }
  layerIso->nVertices++;

  return layerIso->nVertices-1;
}
	 


