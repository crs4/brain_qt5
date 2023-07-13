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
#include "bsIsoBrainViewer.h"
#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsRegion.h"
#include "bsCortexRegion.h"
#include "bsSubRegion.h"
#include "bsSectionRenderer.h"
#include "bsNeuronRenderer.h"

#include "qtCommandHistory.h"
#include "bsCmdCurrentSection.h"
#include "bsCmdUpDownSection.h"
#include "bsDocument.h"
#include <iostream>
#include "sl/clock.hpp"


extern "C" {
#include "mc.h" // MARCHING CUBES ...
};

#define INTERPOLATE 0 
/// WAS 0

#define TRI_RENDER 1
#define DEBUGTESSELLATE 0


////////////////////////////////////

class mc_coded_isosurface {
protected:
  mc_isosurface*   m_iso;
  std::vector<int> m_vertex_code;
public:
  
  mc_coded_isosurface() {
    m_iso = NULL;
  }

  ~mc_coded_isosurface() {
    if (m_iso) mc_iso_free(m_iso);
    m_iso = NULL;
  }

  void set_iso(mc_isosurface *iso) {
    if (m_iso) mc_iso_free(m_iso);
    m_iso = iso;
    if (iso) {
      m_vertex_code.resize(iso->nVertices);
    } else {
      m_vertex_code.clear();
    }
  }
    
  mc_isosurface* iso() {
    return m_iso;
  }

  int vertex_code(int i) const {
    assert(i>=0 && i< (int)m_vertex_code.size());
    return m_vertex_code[i];
  }

  void set_vertex_code(int v, int i) {
    assert(i>=0 && i< (int)m_vertex_code.size());
    m_vertex_code[i] = v;
  }

};

/////////////////////////////////////


static sl::fixed_size_point<3,GLdouble> combined_vertices[100];
static int combined_vertices_count;

static void tessInitCombine() {
  combined_vertices_count=0;
}

static void tessCombine( GLdouble coords[3], 
			 GLdouble *d[4],
			 GLfloat w[4], 
			 GLdouble **dataOut ) {
    assert(combined_vertices_count< 100);

#ifndef NDEBUG
    std::cerr << "Tess: vertex combination #" << combined_vertices_count+1 << std::endl;
#endif
	    
    combined_vertices[combined_vertices_count][0]=coords[0];
    combined_vertices[combined_vertices_count][1]=coords[1];
    combined_vertices[combined_vertices_count][2]=coords[2];
    *dataOut = combined_vertices[combined_vertices_count].to_pointer();

    combined_vertices_count++;    
}

static void tessError(GLenum errno) {
#if GLU_1_2
    std::cerr << "!!!! Tessellation error: " << gluErrorString(errno) << std::endl;
#endif
}


#if DEBUGTESSELLATE

static void myVertex3dv(GLdouble *p) {
  std::cerr << "  p = " << p[0] << " " << p[1] << " " << p[2] << std::endl;
  glVertex3dv(p);
}

static void myBegin(GLenum tp) {
   std::cerr << "glBegin( ";
   switch (tp) {
   case GL_TRIANGLE_FAN: std::cerr << "GL_TRIANGLE_FAN"; break;
   case GL_TRIANGLE_STRIP: std::cerr << "GL_TRIANGLE_STRIP"; break;
   case GL_TRIANGLES: std::cerr << "GL_TRIANGLES"; break;
   default: std::cerr << "UNKNOWN TAG"; break;
   }
   std::cerr << ")" << std::endl;
   glBegin(tp);
}

static void myEnd() {
  std::cerr << "glEnd()" << std::endl;
  glEnd();
}

#endif




static sl_color3ub region_index_to_color(int i) {
  int result;
  if (i<0) {
    result = 255;
  } else {
    result = 16*(i+1);
  }
#ifndef NDEBUG
  std::cerr << "           region_index_to_color(" << i << ") = " << result << std::endl;
#endif
  return sl_color3ub(result, 0, 0);
}

static int red_to_region_index(int r) {
  int result;

  if (r == 255) {
    result = -1;
  } else {
    result = (r+8)/16-1;
    if (result<0) {
#if !INTERPOLATE
      std::cerr << "ERROR:       red_to_region_index(" << r << ") = " << result << std::endl;
#endif
      result=0;
    }
  }

  // std::cerr << "           red_to_region_index(" << r << ") = " << result << std::endl;

  return result;
}

// TODO: controllare m_brain e m_enable_subregions e confrontare con bsFastBrainViewer


bsIsoBrainViewer::bsIsoBrainViewer( QWidget* parent, const char* name, const QGLWidget* shareWidget) 
  : bsBrainViewer( parent, name, shareWidget ) {
    m_document=NULL;
    m_render= new bsSectionRenderer();
    m_enable_subregions = false;
}

bsIsoBrainViewer::~bsIsoBrainViewer() {
}


// Slots


void bsIsoBrainViewer::handleBrainChange() {
#ifndef NDEBUG
  std::cerr << "BEGIN 3D: ISO handleBrainChange" << std::endl;
#endif

  m_brain = m_document->getBrain2D();

  // Remove old brain reconstruction and reallocate
  iso_init();

  // Recenter camera
  if (m_brain && m_brain->sectionCount()>1) {
#ifndef NDEBUG
    std::cerr << "  ### SUBCOLORS RESIZED TO " << m_brain->maxSubRegionId()+1 << std::endl;
#endif

    m_render->resizeSubRegionColors(m_brain->maxSubRegionId()+1);
    m_render->resizeNeuronTypeColors(m_brain->maxNeuronType()+1);
    
    sl::aabox3f v_box = m_brain->boundingBox();
    setInitBox(v_box);
    reset();
  }

#ifndef NDEBUG
  std::cerr << "END 3D: ISO handleBrainChange" << std::endl;
#endif
  updateGL();
}

void bsIsoBrainViewer::rebuild(){
#ifndef NDEBUG
  std::cerr << "**********Begin: ISO rebuild" << std::endl;
#endif
  iso_init();
  updateGL();
#ifndef NDEBUG
  std::cerr << "**********End: ISO rebuild" << std::endl;
#endif
}

void bsIsoBrainViewer::handleSectionChange(int i){
#ifndef NDEBUG
  std::cerr << "BEGIN 3D: ISO handleSectionChange" << std::endl;
#endif

  bsSectionedBrain* b = m_document->getBrain2D();
  if (!b || b->sectionCount()-1 !=  (int)m_iso_slices.size()) {
    // This is a different brain! 
    iso_init();
  } else {
    for (int delta_k=0; delta_k<4; delta_k++) {
      int k = i - 1 + delta_k;
      if (k>=0 && k<(int)m_iso_slices.size()) {
	iso_damage(k);
      }
    }
  }
#ifndef NDEBUG
  std::cerr << "END 3D: ISO handleSectionChange" << std::endl;
#endif

  updateGL();
}

void bsIsoBrainViewer::setEnableSubRegions(bool b) {
  m_enable_subregions = b;
  updateGL();
}

void bsIsoBrainViewer::localPaintGL(){

#ifndef NDEBUG
  std::cerr << "BEGIN 3D: ISO paintGL" << std::endl;
#endif

  if (!isVisible()) return;
  iso_repair();

  // Draw isosurfaces

  bsSectionedBrain* brain2D = m_document->getBrain2D();
  if (brain2D) {
    m_render->setBorderSize(2.0f);

    glPushMatrix(); {
      for (int i=0; i<(int)m_iso_slices.size(); i++) {
	iso_render(i);
      }
    } glPopMatrix();
      
      m_render->setBorderSize(1.0f);
    }

#ifndef NDEBUG
  std::cerr << "END 3D: ISO paintGL" << std::endl; 
#endif

}


void  bsIsoBrainViewer::iso_init() {
  iso_clear();
  if (m_brain && m_brain->sectionCount()-1 > 0) {
#ifndef NDEBUG
    std::cerr << "  ISO3D: Allocating " << m_brain->sectionCount()-1 << " new sections" << std::endl;
#endif
    m_iso_slices.resize(m_brain->sectionCount()-1);
    std::fill(m_iso_slices.begin(), m_iso_slices.end(), (mc_coded_isosurface*)NULL);
  }
}  

void  bsIsoBrainViewer::iso_clear() {
#ifndef NDEBUG
  std::cerr << "  ISO3D: Clearing " << m_iso_slices.size() << " sections" << std::endl;
#endif
  
  for (int i=0; i<(int)m_iso_slices.size(); i++) {
    delete m_iso_slices[i];
    m_iso_slices[i] = NULL;
  }
  m_iso_slices.clear();
} 


void  bsIsoBrainViewer::iso_repair() {
#ifndef NDEBUG
  std::cerr << "  ISO3D: Start Repairing " << m_iso_slices.size() << " sections" << std::endl;
#endif

  bool plane_enabled[6];
  for (int i=0; i<6; i++) {
    plane_enabled[i] = (bool)glIsEnabled(GL_CLIP_PLANE0+i);
    glDisable(GL_CLIP_PLANE0+i);
  }

  sl::real_time_clock clk;
  clk.restart();

  int rebuilt_count = 0;
  int tri_count = 0;
  for (int i=0; i<(int)m_iso_slices.size(); i++) {
    if (m_iso_slices[i] == NULL) {
      rebuilt_count++;
      iso_rebuild(i);
    }
    tri_count += m_iso_slices[i]->iso()->nTriangles;
  }
  sl::time_duration elapsed = clk.elapsed();
  
  for (int i=0; i<6; i++) {
    if (plane_enabled[i]) {
      glEnable(GL_CLIP_PLANE0+i);
    }
  }

  if (rebuilt_count > 0) {
#ifndef NDEBUG
    std::cerr << "  ISO3D: Rebuilt " << 
      rebuilt_count << " isosurface slices in " << elapsed.as_microseconds()/1000.0 << " ms." << std::endl;
    std::cerr << "  ISO3D: Isosurface has " << 
      tri_count << " triangles." << std::endl;
#endif
  }

#ifndef NDEBUG
  std::cerr << "  ISO3D: End Repairing " << m_iso_slices.size() << " sections" << std::endl;
#endif

}


void  bsIsoBrainViewer::iso_damage(int i) {

  if (m_iso_slices[i]) {
#ifndef NDEBUG
    std::cerr << "  ISO3D: Damaging section " << i << std::endl;
#endif

    delete m_iso_slices[i];
    m_iso_slices[i] = NULL;  
  }
}


void  bsIsoBrainViewer::iso_rebuild(int slice_i) {
#ifndef NDEBUG
  std::cerr << "  ISO3D: Rebuilding section " << slice_i << std::endl;
#endif
  
  const int voxel_i = 128; // 192
  const int voxel_j = 128; // 192
  const int voxel_k = 4;
  
  const int section_n = m_iso_slices.size()+1;
  
  std::vector<mc_datum>   dataset = std::vector<mc_datum>(voxel_i*voxel_j*voxel_k);
  std::vector<GLubyte>    buffer  = std::vector<GLubyte>(voxel_i*voxel_j);
  
  
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  GLboolean glwasdithering;
  glGetBooleanv(GL_DITHER, &glwasdithering);
  if (glwasdithering == GL_TRUE) {
      glDisable(GL_DITHER);
  }
  glViewport(0, 0, voxel_j, voxel_i);
  glMatrixMode(GL_PROJECTION);

  sl::aabox3f brain_bbox = m_brain->boundingBox();

  {
    
#ifndef NDEBUG
    std::cerr << "brain_bbox: " << brain_bbox << std::endl;
#endif

    sl::column_vector3f voxel; 
    voxel = 
      (brain_bbox[1][0]-brain_bbox[0][0])/(float)voxel_i,
      (brain_bbox[1][1]-brain_bbox[0][1])/(float)voxel_j, 
      brain_bbox.center()[2];

    brain_bbox.merge(brain_bbox[0]-voxel);
    brain_bbox.merge(brain_bbox[1]+voxel);

#ifndef NDEBUG
    std::cerr << "new brain_bbox: " << brain_bbox << std::endl;
#endif

    glLoadIdentity();
    glOrtho(brain_bbox[0][0], brain_bbox[1][0],
	    brain_bbox[0][1], brain_bbox[1][1],
	    -1.0, 1.0);
  }
  
  // ORTHO bounding box
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  for (int delta_k=0; delta_k<voxel_k; delta_k++) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    int section_i = slice_i - 1 + delta_k;
    if (section_i >=0 && section_i < section_n) {
      // Good section
      section_draw_coded_red(section_i);
    }
    
    glReadBuffer(GL_BACK_LEFT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, voxel_i, voxel_j, GL_RED, GL_UNSIGNED_BYTE, &(buffer[0]));
    
    {
#ifndef NDEBUG
      int n_in = 0;
      int n_out = 0;
#endif
      for (int i=0; i<voxel_i; i++) {
	for (int j=0; j<voxel_j; j++) {	  
	  //dataset[MC_OFFSET(i,j,delta_k,voxel_i,voxel_j)] = buffer[i*voxel_j+j];
	  dataset[MC_OFFSET(i,j,delta_k,voxel_i,voxel_j)] = buffer[j*voxel_i+i];
#ifndef NDEBUG
	  //if (buffer[i*voxel_j+j] == 0) {
	  if (buffer[j*voxel_i+i] == 0) {
	    n_out++;
	  } else {
	    n_in++;
	  }
#endif
	}
      }
      
#ifndef NDEBUG
      std::cerr << "  ISO 3D: " << n_in << "/" << n_in+n_out << " pixels in layer " << delta_k << std::endl;
#endif
    }
  }  
  glViewport(0, 0, width(), height());
  
  mc_isosurface* iso = mc_march_disconnected_layer(&(dataset[0]), 
						   voxel_i,
						   voxel_j,
						   voxel_k,
						   1,
						   1./255.0,
						   INTERPOLATE);
  if (slice_i == 0) {
    mc_isosurface* iso2 =  mc_march_disconnected_layer(&(dataset[0]), 
						   voxel_i,
						   voxel_j,
						   voxel_k,
						   0,
						   1./255.0,
						   INTERPOLATE);
    mc_iso_append(iso, iso2);
    mc_iso_free(iso2);
  } else if (slice_i == section_n) {
    mc_isosurface* iso2 =  mc_march_disconnected_layer(&(dataset[0]), 
						   voxel_i,
						   voxel_j,
						   voxel_k,
						   2,
						   1./255.0,
						   INTERPOLATE);
    mc_iso_append(iso, iso2);
    mc_iso_free(iso2);
  }

  mc_coded_isosurface *result = new mc_coded_isosurface;
  result->set_iso(iso);
  
  /* REMAP to bbox: 0->box_min, voxel_i->box_max. ... */
  {
    bsSection* s = m_brain->sectionItem(slice_i);
    
    sl::point3f pmin = sl::point3f(brain_bbox[0][0],
				   brain_bbox[0][1],
				   s->getQuota());
    sl::point3f pmax = sl::point3f(brain_bbox[1][0],
				   brain_bbox[1][1],
				   s->getQuota()+2.0f*s->getUpThickness());
    sl::aabox3f bbox(pmin, pmax);
    
    sl::vector3f scale;
    sl::vector3f translate;
    
    scale[0] = (1./(float)(voxel_i-1))*(bbox[1][0]-bbox[0][0]);
    scale[1] = (1./(float)(voxel_j-1))*(bbox[1][1]-bbox[0][1]);
    scale[2] = (1./(float)(1))        *(bbox[1][2]-bbox[0][2]);
    
    translate[0] = bbox[0][0];
    translate[1] = bbox[0][1];
    translate[2] = s->getQuota()-2.0f*s->getUpThickness();
    
    sl::vector3f normal_scale;
    
    normal_scale[0] = 1.0f/scale[0];
    normal_scale[1] = 1.0f/scale[1];
    normal_scale[2] = 1.0f/scale[2];
    
    for (int i=0; i<result->iso()->nVertices; i++) {
      int vi[3];
      vi[0] = (int)result->iso()->vertices[0][i];
      vi[1] = (int)result->iso()->vertices[1][i];
      vi[2] = (int)result->iso()->vertices[2][i];
      
      int code = red_to_region_index(dataset[MC_OFFSET(vi[0],vi[1],vi[2],voxel_i,voxel_j)]);
      //int code = red_to_region_index(dataset[MC_OFFSET(vi[0],vi[1],vi[2],voxel_j,voxel_i)]);
      // RAYCAST INSTEAD?
      if (code > m_brain->maxSubRegionId()) {
#if !INTERPOLATE
	std::cerr << "ERROR: code= " << code << std::endl;
#endif
	code = m_brain->maxSubRegionId();
      }
      result->set_vertex_code(code, i);
      
      float s = 0.0;
      for (int d=0; d<3; d++) {
	iso->vertices[d][i] = result->iso()->vertices[d][i] * scale[d] + translate[d];
	iso->normals[d][i] = result->iso()->normals[d][i] * normal_scale[d];
	s += iso->normals[d][i] * iso->normals[d][i];
      }
      s = std::sqrt(s);
      for (int d=0; d<3; d++) {
	iso->normals[d][i] /= s;
      }
    }    
  }
  
  if (m_iso_slices[slice_i]) {
    delete m_iso_slices[slice_i];
  }
  m_iso_slices[slice_i] = result;

  if (glwasdithering == GL_TRUE) {
      glEnable(GL_DITHER);
  }

#ifndef NDEBUG  
  std::cerr << "Rebuilt iso for slice " << slice_i << ": " << 
    result->iso()->nVertices << " verts " <<
    result->iso()->nTriangles << " tris " <<
    std::endl;
#endif
}




void bsIsoBrainViewer::iso_render(int slice_i) {
  // Loop on triangles of iso i
  mc_coded_isosurface* coded_iso = m_iso_slices[slice_i];
  mc_isosurface* iso = coded_iso->iso();

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glBegin(GL_TRIANGLES); {
    glColor3fv(m_render->externalColor().to_pointer());
    for (int i=0; i<iso->nTriangles; i++) {
	// NOTE: ordering is clockwise, normals towards interior!
	for (int d=0; d<3; d++) {
	  int k = iso->triangles[d][i];
	  if (m_enable_subregions) {
	    int code = coded_iso->vertex_code(k);
	    if (code<0) {
	      glColor3fv(m_render->externalColor().to_pointer());
	    } else {
	      glColor3fv(m_render->subRegionColor(code).to_pointer());
	    }
	  }
	  glNormal3f(-iso->normals[0][k], -iso->normals[1][k], -iso->normals[2][k]);
	  glVertex3f(iso->vertices[0][k], iso->vertices[1][k], iso->vertices[2][k]);
	}
    }
  } glEnd();
      
  glFrontFace(GL_CCW);

#define DRAW_NORMALS 0
#if DRAW_NORMALS

  float normal_lenght = m_brain->boundingBox().diagonal().two_norm()/100.;
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES); {
    glColor3f(0.1, 0.1, 0.9);
    for (int k=0; k<iso->nVertices; k++) {

      glVertex3f(iso->vertices[0][k], 
		 iso->vertices[1][k], 
		 iso->vertices[2][k]);
      glVertex3f((iso->vertices[0][k]-normal_lenght*iso->normals[0][k]), 
		 (iso->vertices[1][k]-normal_lenght*iso->normals[1][k]), 
		 (iso->vertices[2][k]-normal_lenght*iso->normals[2][k]));
    }
  } glEnd();
  glEnable(GL_LIGHTING);
#endif


  glDisable(GL_CULL_FACE);

#ifndef NDEBUG
  std::cerr << "Rendered iso for slice " << slice_i << ": " << 
    iso->nVertices << " verts " << 
    iso->nTriangles << " tris " <<
    std::endl;
#endif
}


void bsIsoBrainViewer::section_draw_coded_red(int section_i) {

  bsSection *section = m_brain->sectionItem(section_i);

  for (int i=0; i<section->cortexRegionCount(); i++) {
    const bsCortexRegion *cort_reg = section->cortexRegionItem(i);
    int n_sub = cort_reg->subRegionCount();
    if (n_sub == 0) {  // there is only a cortex region
      render_region(cort_reg, -1);
    } else { // each cortex region is made of sub regions
      for (int j=0; j<n_sub; j++) {
	const bsSubRegion *sub_reg = cort_reg->subRegionItem(j);
	render_region(sub_reg, sub_reg->getId());
      }
    }
  }
}


void bsIsoBrainViewer::render_region(const bsRegion* parsed_region, int region_i) {


#if TRI_RENDER
  // triangle filling
  triangle_render(parsed_region, region_i);
#else
  // glu tessellator
  tessel_render(parsed_region, region_i);
#endif // TRI_RENDER

}


void bsIsoBrainViewer::triangle_render(const bsRegion* parsed_region, int region_i) {
  int n_tri = parsed_region->triangleCount();
  glColor3ubv(region_index_to_color(region_i).to_pointer());

  glBegin(GL_TRIANGLES);
  {
    for(int i=0; i<n_tri; i++) {
      glVertex2fv(parsed_region->triangleVertexItem(i,0).to_pointer());
      glVertex2fv(parsed_region->triangleVertexItem(i,1).to_pointer());
      glVertex2fv(parsed_region->triangleVertexItem(i,2).to_pointer());
    }
  }
  glEnd();
}


void bsIsoBrainViewer::tessel_render(const bsRegion* parsed_region, int region_i) {


  static GLUtriangulatorObj *tobj = NULL;

  if (tobj == NULL) {
    tobj = gluNewTess();  /* create and initialize a GLU
			     polygon tessellation object */
#if DEBUGTESSELLATE
    gluTessCallback(tobj, GLU_TESS_BEGIN, (GLvoid (*)())myBegin);
    gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid (*)())myVertex3dv);
    gluTessCallback(tobj, GLU_TESS_END, (GLvoid (*)())myEnd);
    gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (*)())tessCombine);
    gluTessCallback(tobj, GLU_TESS_ERROR, (GLvoid (*)())tessError);
#else
    gluTessCallback(tobj, GLU_TESS_BEGIN, (GLvoid (*)())glBegin);
    gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid (*)())glVertex3dv);
    gluTessCallback(tobj, GLU_TESS_END, (GLvoid (*)())glEnd);
    gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (*)())tessCombine);
    gluTessCallback(tobj, GLU_TESS_ERROR, (GLvoid (*)())tessError);
#endif

    // tessellation routine
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wire frame view

    glColor3ubv(region_index_to_color(region_i).to_pointer());

    tessInitCombine();


    int n_holes = parsed_region->holeCount();
    std::vector<sl::fixed_size_point<3,GLdouble> *> tmp(n_holes+1);

#if GLU_1_2
    gluTessBeginPolygon(tobj,NULL); {

      gluTessBeginContour(tobj); {
 
	int n_ext = parsed_region->externalBoundaryVertexCount();
	tmp[0] = new sl::fixed_size_point<3,GLdouble>[n_ext];

	for (int k=0; k<n_ext; k++) {
	  sl::point2f p2 = parsed_region->externalBoundaryVertexItem(k);
	  tmp[0][k][0] = (GLdouble)p2[0];
	  tmp[0][k][1] = (GLdouble)p2[1];
	  tmp[0][k][2] = (GLdouble)0.0;
	  gluTessVertex(tobj, (GLdouble*)(&(tmp[0][k])), (GLdouble*)(&(tmp[0][k]))); 
#if DEBUGTESSELLATE
	  std::cerr << "glTessVertex(" << (GLdouble*)(&(tmp[0][k])) << "," << (GLdouble*)(&(tmp[0][k])) <<  ")" << std::endl; 
#endif
	}
      } gluTessEndContour(tobj);

      // holes
      int n_holes = parsed_region->holeCount();
      for (int l=0; l<n_holes; l++) {
	int n_hv = parsed_region->holeVertexCount(l);
	tmp[l+1] = new sl::fixed_size_point<3,GLdouble>[n_hv];
	tessInitCombine();
	gluTessBeginContour(tobj); {
#if DEBUGTESSELLATE
	  std::cerr << "HOLE: vector size: " << n_hv << std::endl;
#endif
	  for (int k=0; k<n_hv; k++) {
	    sl::point2f p2 = parsed_region->holeVertexItem(l,k);
	    tmp[l+1][k][0] = (GLdouble)p2[0];
	    tmp[l+1][k][1] = (GLdouble)p2[1];
	    tmp[l+1][k][2] = (GLdouble)0.0;
	    gluTessVertex(tobj, (GLdouble*)(&(tmp[l+1][k])), (GLdouble*)(&(tmp[l+1][k]))); 
#if DEBUGTESSELLATE
	    std::cerr << "glTessVertex(" << (GLdouble*)(&(tmp[l+][k])) << "," << (GLdouble*)(&(tmp[l+1][k])) << ")" << std::endl; 
#endif
	  } 
	} gluTessEndContour(tobj); // ith hole
      }
    } gluTessEndPolygon(tobj);

    for (int l=0; l<1+n_holes; l++) {
      delete [] tmp[l];
    }
#endif  // GLU_1_2
  }
}





