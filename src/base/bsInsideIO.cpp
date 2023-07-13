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
#include "bsInsideIO.h"
#include "bsSectionIO.h"
#include "bsSection.h"
#include "bsGeometryHelpers.h"
#include "bsCortexRegion.h"
#include "bsSubRegion.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <set>

// Check endiannes
static bool endian_check() {
  union {
    unsigned char b[4];
    unsigned long i;
  } decode;

  decode.i = 1;
  if (decode.b[0] == 0) {
    return true;
  } else {
    return false;
  }
}

// Creation & Destruction
 
bsInsideIO::bsInsideIO() {
   //  const float VGAP = 3000.;
  const float VGAP = 4000.;
  m_vgap = VGAP;
}
  
bsInsideIO::~bsInsideIO() {
}

// Internal Functions

static void readint16 (FILE *in, int *nitem, bool *ok) {
  unsigned char item[4];
  *ok = fread (item, 1, 2, in) == 2;
  if (*ok) {
    *nitem = item[0] + (item[1] << 8);
  }
}

static void readfloat (FILE *in, float *nitem, bool *ok) {
  union {
    unsigned 
    char b[4];
    float val;
  } decode;

  char item[6];
  *ok = fread (item,1,4,in) == 4;
  if (*ok) {
    if (endian_check()) {
      decode.b[0] = item[3];
      decode.b[1] = item[2];
      decode.b[2] = item[1];
      decode.b[3] = item[0];
    } else {
      decode.b[0] = item[0];
      decode.b[1] = item[1];
      decode.b[2] = item[2];
      decode.b[3] = item[3];
    }
    *nitem = decode.val;
  }
}

static void readpoint(FILE* in, sl::point2f* p, bool *ok) {
  readfloat(in, &((*p)[0]), ok);
  if (*ok) {
    readfloat(in, &((*p)[1]), ok);
  }
}

static void readconnectivity(FILE* in,
			     std::vector< sl::point2f >&    points,
			     std::vector< int >&          boundary_connectivity,
			     std::vector< std::vector<int> >&  holes_or_boundaries,
			     const float             vgap,
			     bool *ok) {
  *ok = true;

  int n;
  readint16 (in, &n, ok);
  if (!*ok) return;
  *ok = n >= 3;
  if (!*ok) return;

  int nh = holes_or_boundaries.size();
  int current_pt = 0;
  std::set<sl::point2f> point_set;
  for(int i=0; i<n; i++) {
    sl::point2f p;
    readpoint(in, &p, ok);
    if (!*ok) return;
    std::set<sl::point2f>::iterator it;
    it=point_set.find(p);
    if (it == point_set.end()) {
      points.push_back(p);
      point_set.insert(p);
      int np = points.size()-1;
      if (i<3) {
	boundary_connectivity.push_back(np);
	current_pt ++;
      } else if (current_pt >= 3 &&
		 p.distance_squared_to(points[points.size()-2]) > vgap*vgap) {
	std::vector<int> c;
	c.push_back(np);
	holes_or_boundaries.push_back(c);
	current_pt = 1;
      } else if ((int)holes_or_boundaries.size() == nh) {
	boundary_connectivity.push_back(np);
	current_pt ++;
      } else {
	holes_or_boundaries[holes_or_boundaries.size()-1].push_back(np);
	current_pt ++;
      }
    } 
  }

  if (nh != (int)holes_or_boundaries.size() &&
      holes_or_boundaries[holes_or_boundaries.size()-1].size() < 3) {
    holes_or_boundaries.pop_back();
  }

  *ok = bsGeometryHelpers::signedArea(boundary_connectivity,
		   points) != 0.0f;
  {
    for (int i= nh; i<(int)holes_or_boundaries.size() && *ok; i++) {
      *ok = bsGeometryHelpers::signedArea(holes_or_boundaries[i], points) != 0.0f;
    }
  }

}

// Load  

bsSection* bsInsideIO::load(const std::string& fname){
#define ABORT_ON_ERROR(_ok, _code) { \
  if (!(_ok)) {\
    setLastErrorCode(_code);\
    if (result) delete result;\
    if (in) fclose(in);\
    return NULL;\
  }\
}
  
  setCurrentFileName(fname);

  bsSection* result = NULL;

  FILE *in  = fopen (fname.c_str(), "rb");  

  ABORT_ON_ERROR(in != NULL, IO_ERR_FILE_NOT_FOUND);
    
  std::vector< sl::point2f >    points;
  std::vector< int >          external_boundary_connectivity;
  std::vector< int >          internal_boundary_connectivity;
  std::vector< std::vector<int> >  holes_or_boundaries;
  std::vector< sl::tuple2i >    cuts;
  std::vector< int >          region_ids;
  
  bool ok = true;
  
  // Load External Boundary & Holes
  {
    readconnectivity(in, points, external_boundary_connectivity, holes_or_boundaries, getVgap(), &ok);
    ABORT_ON_ERROR(ok, IO_ERR_READ_EXTERNAL_BOUNDARY);
  }

  // Load Internal Boundary & Holes
  {
    readconnectivity(in, points, internal_boundary_connectivity, holes_or_boundaries, getVgap(), &ok);
    ABORT_ON_ERROR(ok, IO_ERR_READ_INTERNAL_BOUNDARY);
  }

  // Skip reflection data
  {
    int n_refl;
    readint16 (in, &n_refl, &ok);
    if (ok) {
      ok = fseek (in, 8*sizeof(float)*n_refl, SEEK_CUR) == 0;
    }
    ABORT_ON_ERROR(ok,IO_ERR_READ_REFL_DATA);
  }

  // Read cyto borders
  {
    int n_cyto;
    readint16 (in, &n_cyto, &ok);
    ABORT_ON_ERROR(ok,IO_ERR_READ_CYTO_DATA);

    if ( n_cyto > 0) {
      std::vector<sl::point2f> p[2];
      p[0].resize(n_cyto);
      p[1].resize(n_cyto);
      for (int k=0; k<2; k++) {
	int i;
	for (i=0; i<n_cyto; i++) {
	  float f;
	  readfloat(in, &f, &ok);
	  ABORT_ON_ERROR(ok,IO_ERR_READ_CYTO_DATA);
	  p[k][i][0]=f;
	}
	for (i=0; i<n_cyto; i++) {
	  float f;
	  readfloat(in, &f, &ok);
	  ABORT_ON_ERROR(ok,IO_ERR_READ_CYTO_DATA);
	  p[k][i][1]=f;
	}
	for (i=0; i<n_cyto; i++) {
	  float f;
	  readfloat(in, &f, &ok);
	  ABORT_ON_ERROR(ok,IO_ERR_READ_CYTO_DATA);
	}
	for (i=0; i<n_cyto; i++) {
	  float f;
	  readfloat(in, &f, &ok);
	  ABORT_ON_ERROR(ok,IO_ERR_READ_CYTO_DATA);
	}
      }

      for (int i=0; i<n_cyto; i++) {
#ifndef NDEBUG
	std::cerr << "Cut: " << i << 
	  " (" << p[0][i][0] << ", " << p[0][i][1] << ")" <<
	  " ->" <<
	  " (" << p[1][i][0] << ", " << p[1][i][1] << ")" <<
	  std::endl;
#endif	
	sl::tuple2i cut;
	sl::point2f o = p[0][i];
	sl::point2f e = p[1][i];

	bsGeometryHelpers::polygonIntersection i0 =
	  bsGeometryHelpers::intersect(external_boundary_connectivity,
				       points,
				       o,e);
	cut[0]=i0.closestVertexIndex();
	bsGeometryHelpers::polygonIntersection i1 =
	  bsGeometryHelpers::intersect(internal_boundary_connectivity,
				       points,
				       e,o);
	cut[1]=i1.closestVertexIndex();

	if (cut[0] >= 0 && cut[0] < (int)external_boundary_connectivity.size() &&
	    cut[1] >= 0 && cut[1] < (int)internal_boundary_connectivity.size()) {
#ifndef NDEBUG
	  std::cerr << "load: Correct cut: " << cut[0] << ", " << cut[1] << std::endl;
#endif
	  cuts.push_back(cut);
	  region_ids.push_back(i);
	} else {
#ifndef NDEBUG
	  std::cerr << "load: Wrong cut: " << cut[0] << ", " << cut[1] << std::endl;
#endif
	}
      }

      if (cuts.size() == 1) {
#ifndef NDEBUG
	std::cerr << "load: Only one cut!" << std::endl;
#endif
	cuts.clear();
	region_ids.clear();
      }
    }
  }
    
  // Fare nuovo creatore di bsSection che tiene conto di
  // di corteccia e buchi

  result = new bsSection(points,
			 external_boundary_connectivity,
			 internal_boundary_connectivity,
			 holes_or_boundaries,
			 cuts,
			 region_ids);
  ABORT_ON_ERROR(result,IO_ERR_CREAT_SECTION); 
      
  // Read neurons
  {
    for (int tp=0; tp<6; tp++) {
      int n_tp;
      readint16(in, &n_tp, &ok);
      ABORT_ON_ERROR(ok,IO_ERR_READ_NEURON_TYPE_COUNTER);
      for (int i=0; i<n_tp; i++) {
	sl::point2f p;
	readpoint(in, &p, &ok);
	ABORT_ON_ERROR(ok,IO_ERR_READ_NEURON);
	result->appendNeuron(tp, p);
      }
    }

    result->assignNeurons();
  }

  // Quota
  {
    float z;
    readfloat(in, &z, &ok);
    ABORT_ON_ERROR(ok, IO_ERR_READ_QUOTA);
    result->setQuota(z);
  }

  fclose(in);
  
  setLastErrorCode(IO_OK);

  return result;

#undef ABORT_ON_ERROR
}

// SAVE

// Internal Functions

static void writeint16 (FILE *out, int nitem, bool *ok) {
    unsigned char b[2];
    b[0] = nitem%256;
    b[1] = nitem/256;
    *ok = fwrite(b, 1, 2, out) == 2;
}

static void writefloat (FILE *out, float nitem, bool *ok) {
    union {
	unsigned 
	char b[4];
	float val;
    } decode;
    
    decode.val=nitem;
    
    char item[4];
    if (endian_check()) {
      item[3]=decode.b[0];
      item[2]=decode.b[1];
      item[1]=decode.b[2];
      item[0]=decode.b[3];
    } else {
      item[0]=decode.b[0];
      item[1]=decode.b[1];
      item[2]=decode.b[2];
      item[3]=decode.b[3];
    }      
    
    *ok = fwrite(item,1,4,out) == 4;
}

static void writepoint(FILE* out, sl::point2f p, bool *ok) {
    writefloat(out, p[0], ok);
    if (*ok) {
	writefloat(out, p[1], ok);
    }
}


void bsInsideIO::save(const std::string& fname, const bsSection* s){
#define ABORT_ON_ERROR(_ok, _code) { \
  if (!(_ok)) {\
    setLastErrorCode(_code);\
    if (out) fclose(out);\
    return;\
  }\
}
     
    assert(s);
    assert(s->cortexRegionCount()>0);
    setCurrentFileName(fname);

    FILE *out  = fopen (fname.c_str(), "wb");  
    ABORT_ON_ERROR(out != NULL, IO_ERR_FILE_NOT_OPEN);
    
    bool ok;

    // External Cortex
    const bsCortexRegion* main_cortex = s->cortexRegionItem(0);
    {
	{
	    ok=main_cortex->hasInternalBoundary();
	    ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	}
	int n_ext=0;
	{
	    for (int nc=0; nc<s->cortexRegionCount(); nc++) {
		n_ext += s->cortexRegionItem(nc)->externalBoundaryVertexCount();
	    }
	    for (int nh=0; nh<main_cortex->holeCount(); nh++) {
		if (nh != main_cortex->getInternalBoundaryIndex()) {
		    n_ext += main_cortex->holeVertexCount(nh);
		}
	    }
	}
#ifndef NDEBUG
	std::cerr << "DEBUG n_ext: " << n_ext << std::endl;
#endif
	{
	    writeint16(out,n_ext,&ok);
	    ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	}
	{
	    for (int i=0; i<main_cortex->externalBoundaryVertexCount(); i++) {
		sl::point2f point=main_cortex->externalBoundaryVertexItem(i);
		writepoint(out,point,&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	    for (int nc=1; nc<s->cortexRegionCount(); nc++) {
		const bsCortexRegion* c = s->cortexRegionItem(nc);
		for (int j=0; j<c->externalBoundaryVertexCount(); j++) {
		    sl::point2f point=c->externalBoundaryVertexItem(j);
		    writepoint(out,point,&ok);
		    ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
		}
	    }
	}
	{
	    for (int nh=0; nh<main_cortex->holeCount(); nh++) {
		if (nh != main_cortex->getInternalBoundaryIndex()) {
		    for (int i=0; i<main_cortex->holeVertexCount(nh); i++) {
			sl::point2f point=main_cortex->holeVertexItem(nh,i);
			writepoint(out,point,&ok);
			ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
		    }
		}
	    }
	}
    }

    // Internal Cortex
    {
	{
	    writeint16(out,main_cortex->internalBoundaryVertexCount(),&ok);
	    ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	}
	{
	    for (int i=0; i<main_cortex->internalBoundaryVertexCount(); i++) {
		sl::point2f point=main_cortex->internalBoundaryVertexItem(i);
		writepoint(out,point,&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	}
    }

    // Reflextion Data
    {
	int n_refl = 0;
	writeint16(out,n_refl,&ok);
	ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
    }
	
    // Cyto Data
    {
	writeint16(out,main_cortex->subRegionCount(),&ok);
	ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
#ifndef NDEBUG
	std::cerr << "DEBUG n_cyto: " << main_cortex->subRegionCount() << std::endl;
#endif
	{ // External Border - X
	    for (int ns=0; ns<main_cortex->subRegionCount(); ns++) {
		const bsSubRegion* sr= main_cortex->subRegionItem(ns);
		sl::point2f p_ext=sr->border2VertexItem(0);
		writefloat(out,p_ext[0],&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	}
	{ // External Border - Y
	    for (int ns=0; ns<main_cortex->subRegionCount(); ns++) {
		const bsSubRegion* sr= main_cortex->subRegionItem(ns);
		sl::point2f p_ext=sr->border2VertexItem(0);
		writefloat(out,p_ext[1],&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	}
	{ // External Border - Connectivity
	    for (int ns=0; ns<main_cortex->subRegionCount(); ns++) {
		writefloat(out,0.0f,&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
		writefloat(out,0.0f,&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	}
	{ // Internal Border - X
	    for (int ns=0; ns<main_cortex->subRegionCount(); ns++) {
		const bsSubRegion* sr= main_cortex->subRegionItem(ns);
		sl::point2f p_int=sr->border2VertexItem(sr->border2VertexCount()-1);
		writefloat(out,p_int[0],&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	}
	{ // Internal Border - Y
	    for (int ns=0; ns<main_cortex->subRegionCount(); ns++) {
		const bsSubRegion* sr= main_cortex->subRegionItem(ns);
		sl::point2f p_int=sr->border2VertexItem(sr->border2VertexCount()-1);
		writefloat(out,p_int[1],&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	}
	{ // Internal Border - Connectivity
	    for (int ns=0; ns<main_cortex->subRegionCount(); ns++) {
		writefloat(out,0.0f,&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
		writefloat(out,0.0f,&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	}
    }

    // Neurons
    {
	for (int tp=0; tp<=s->maxNeuronType(); tp++) {
#ifndef NDEBUG
	    std::cerr << "DEBUG n_neuron: " << s->maxNeuronType() << std::endl;	    
	    std::cerr << "DEBUG n_neuron("<< tp << "): " << s->neuronCount(tp) << std::endl;	    
#endif
	    writeint16(out,s->neuronCount(tp),&ok);
	    ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    for (int i=0; i<s->neuronCount(tp);i++) {
		sl::point2f point=s->neuronItem(tp,i);
		writepoint(out,point,&ok);
		ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
	    }
	}
	{
	    for (int tp=s->maxNeuronType()+1; tp<=5; tp++) {
		writeint16(out,0,&ok);
	    }
	}
    }

    // Quota
    {
#ifndef NDEBUG
	std::cerr << "DEBUG Quota: " << s->getQuota() << std::endl;
#endif
	writefloat(out,s->getQuota(),&ok);
	ABORT_ON_ERROR(ok, IO_ERR_WRITE_SECTION);
    }    
	
    fclose(out);
    setLastErrorCode(IO_OK);

}

