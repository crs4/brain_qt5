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
/* mc.h Marching Cubes implementation for lab1
   Mike Chupa
   CS 8413: Visualization
   Instructor: Dr. Raghu Machiraju
   Copyright (c) 1998 Michael A. Chupa Permission is expressly granted
   to use this code in any non-commercial work, provided that this
   notice is preserved. However, be aware that the Marching Cubes
   algorithm is patented.
*/


#ifndef MC_H
#define MC_H

/* macro to do array-style indexing into the scalar field */
#define MC_OFFSET(I, J, K, IDIM, JDIM) ((I) + ((IDIM) * (J)) + ((IDIM * JDIM) * (K)))

/* macro to do array-style indexing into the layer edge table */
#define MC_EDGE_OFFSET(E, I, J, IDIM) ((E) + (12 * (I)) + ((12 * (IDIM-1)) * (J-1)))

typedef unsigned char mc_datum;

typedef struct mc_layer {
  mc_datum *botData;
  mc_datum *topData;
  long  *edges;  /* tag indexing into vertex list */
} mc_layer;

typedef struct mc_isosurface {
    float     *vertices[3];
    float      *normals[3];
    long     *triangles[3];
    long      nVertices;
    long     nTriangles;
} mc_isosurface;  


extern void mc_iso_append(mc_isosurface* iso1, mc_isosurface* iso2);

extern void mc_iso_free(mc_isosurface * iso);

extern void mc_march_layer(mc_datum *dataset, mc_layer *layer, int iDim, int jDim, int kDim, 
			   int k, float threshold, int interpolate, mc_isosurface *iso);

extern mc_isosurface *mc_march_disconnected_layer(mc_datum *dataset, 
						  int iDim, int jDim, int kDim, 
						  int k, float threshold,
						  int interpolate);

extern mc_isosurface *mc_march(mc_datum *dataset, 
			       int iDim, int jDim, int kDim, 
			       float threshold,
			       int interpolate);


#endif
