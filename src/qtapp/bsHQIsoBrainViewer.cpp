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
#include "bsHQIsoBrainViewer.h"
#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsRegion.h"
#include "bsCortexRegion.h"
#include "bsSubRegion.h"

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
      m_vertex_code.resize(std::size_t(iso->nVertices));
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


bsHQIsoBrainViewer::bsHQIsoBrainViewer(QWidget* parent) 
  : bsBrainViewer(parent) {
    m_document=NULL;
    m_enable_subregions = false;
    m_iso_geometry=0;
    m_brain=0;
}

bsHQIsoBrainViewer::~bsHQIsoBrainViewer() {
#ifndef NDEBUG
  std::cerr << "#################### Destroying bsHQIsoBrainViewer" << std::endl;
#endif
  if ( m_iso_geometry ) {
    delete m_iso_geometry;
  }
  m_iso_geometry = NULL;
}

// Document: cutting box is connected to Document
void bsHQIsoBrainViewer::setDocument(bsDocument* doc) {
  bsBrainViewer::setDocument(doc);
  connect(this,SIGNAL(cuttingPlanesChanged(sl::obox3f&)),m_document,SLOT(setCuttingBox(sl::obox3f&)));
  connect(m_document,SIGNAL(newCuttingBoxLoaded()),this,SLOT(setCuttingBoxFromDocument()));
}



// Slots

void bsHQIsoBrainViewer::setCuttingBoxFromDocument() {
  setCuttingBox(m_document->cuttingBox());
}


void bsHQIsoBrainViewer::handleBrainChange() {
#ifndef NDEBUG
  std::cerr << "BEGIN 3D: ISO handleBrainChange" << std::endl;
#endif

  m_brain = m_document->getBrain2D();

  // Remove old brain reconstruction and reallocate
  iso_clear();

  // Recenter camera
  if (m_brain && m_brain->sectionCount()>1) {
#ifndef NDEBUG
    std::cerr << "  ### SUBCOLORS RESIZED TO " << m_brain->maxSubRegionId()+1 << std::endl;
#endif
    sl::aabox3f v_box = m_brain->boundingBox();
    setInitBox(v_box);
    reset();
  }

#ifndef NDEBUG
  std::cerr << "END 3D: ISO handleBrainChange" << std::endl;
#endif
  updateGL();
}

void bsHQIsoBrainViewer::rebuild(){
#ifndef NDEBUG
  std::cerr << "**********Begin: ISO rebuild" << std::endl;
#endif
  iso_clear();
  updateGL();
#ifndef NDEBUG
  std::cerr << "**********End: ISO rebuild" << std::endl;
#endif
}


void bsHQIsoBrainViewer::setEnableSubRegions(bool b) {
  m_enable_subregions = b;
  updateGL();
}

void bsHQIsoBrainViewer::localPaintGL(){

#ifndef NDEBUG
  std::cerr << "BEGIN 3D: ISO paintGL" << std::endl;
#endif

  // Draw isosurfaces

  bsSectionedBrain* brain2D = m_document->getBrain2D();
  if (brain2D) {
    iso_repair();
    glPushMatrix(); {
      iso_render();
    } glPopMatrix();
  }

#ifndef NDEBUG
  std::cerr << "END 3D: ISO paintGL" << std::endl; 
#endif

}


void bsHQIsoBrainViewer::handleSectionChange(int ){
  iso_clear();
}

void  bsHQIsoBrainViewer::iso_clear() {
#ifndef NDEBUG
  std::cerr << "  ISO3D: Clearing " << std::endl;
#endif
  if ( m_iso_geometry ) {
    delete m_iso_geometry;
  }
  m_iso_geometry = NULL;
} 


void  bsHQIsoBrainViewer::iso_repair() {
#ifndef NDEBUG
  std::cerr << "  ISO3D: Start Repairing " << std::endl;
#endif

  bool plane_enabled[6];
  for (int i=0; i<6; i++) {
    plane_enabled[i] = (bool)glIsEnabled(GL_CLIP_PLANE0+i);
    glDisable(GL_CLIP_PLANE0+i);
  }

  sl::real_time_clock clk;
  clk.restart();

  if (m_iso_geometry == NULL) {
    iso_rebuild();
  }

  for (int i=0; i<6; i++) {
    if (plane_enabled[i]) {
      glEnable(GL_CLIP_PLANE0+i);
    }
  }

#ifndef NDEBUG
  std::cerr << "  ISO3D: End Repairing " << std::endl;
#endif

}


void  bsHQIsoBrainViewer::iso_rebuild() {
#ifndef NDEBUG
  std::cerr << "  ISO3D: Rebuilding geometry " << std::endl;
#endif
  const int molt_section = 2;
  const int voxel_i = 128; // 192
  const int voxel_j = 128; // 192
  const int voxel_k = m_brain->sectionCount()*molt_section+2;
  
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
      (brain_bbox[1][2]-brain_bbox[0][2])/(float)voxel_k; 

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
  
  for (int k=0; k<voxel_k; k++) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (k > 0 && k < voxel_k-1) {
      section_draw_coded_red((k-1)/2);
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
	  dataset[MC_OFFSET(i,j,k,voxel_i,voxel_j)] = buffer[j*voxel_i+i];
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
      std::cerr << "  ISO 3D: " << n_in << "/" << n_in+n_out << " pixels in layer " << k << std::endl;
#endif
    }
  }  
  glViewport(0, 0, width(), height());
  if (glwasdithering == GL_TRUE) {
      glEnable(GL_DITHER);
  }

  
  mc_isosurface* iso = mc_march(&(dataset[0]), 
				 voxel_i,
				 voxel_j,
				 voxel_k,
				 1./255.0,
				 INTERPOLATE);

  mc_coded_isosurface *result = new mc_coded_isosurface;
  result->set_iso(iso);
  
  /* REMAP to bbox: 0->box_min, voxel_i->box_max. ... */

  
  {
    sl::aabox3f real_brain_bbox = m_brain->boundingBox();

    sl::vector3f translate;
    sl::vector3f scale;
    translate[0] = (real_brain_bbox[1][0]-real_brain_bbox[0][0]*(voxel_i-2))/(3-voxel_i);
    translate[1] = (real_brain_bbox[1][1]-real_brain_bbox[0][1]*(voxel_j-2))/(3-voxel_j);
    scale[0] =    real_brain_bbox[0][0] - translate[0];
    scale[1] =    real_brain_bbox[0][1] - translate[1];
    if (voxel_k == 3) {
      translate[2] = (real_brain_bbox[1][2]-real_brain_bbox[0][2])/2.0f;
      scale[2] = 2.0f*(real_brain_bbox[1][2]-real_brain_bbox[0][2])/3.0f;
    } else {
      translate[2] = (real_brain_bbox[1][2]-real_brain_bbox[0][2]*(voxel_k-2))/(3-voxel_k);
      scale[2] =    real_brain_bbox[0][2] - translate[2];
    }


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
  
  if (m_iso_geometry) {
    delete m_iso_geometry;
  }
  m_iso_geometry = result;

#ifndef NDEBUG  
  std::cerr << "Rebuilt iso for geometry: " << 
    result->iso()->nVertices << " verts " <<
    result->iso()->nTriangles << " tris " <<
    std::endl;
#endif

  iso_relax();

#ifndef NDEBUG  
  std::cerr << "Relaxed iso for geometry: " << 
    result->iso()->nVertices << " verts " <<
    result->iso()->nTriangles << " tris " <<
    std::endl;
#endif

  iso_recompute_normals();
#ifndef NDEBUG  
  std::cerr << "Recomputed iso normals for geometry: " << 
    result->iso()->nVertices << " verts " <<
    result->iso()->nTriangles << " tris " <<
    std::endl;
#endif


}

void bsHQIsoBrainViewer::iso_recompute_normals() {
  mc_isosurface* iso = m_iso_geometry->iso();

  // Set all normals to zero
  for (int k=0; k<iso->nVertices; ++k) {
    for (int d=0; d<3; ++d) {
      iso->normals[d][k] = 0.0f;
    }
  }

  // Accumulate normals
  for (int i=0; i<iso->nTriangles; ++i) {
    sl::point3f v[3];
    for (int j=0; j<3; ++j) {
      int k = iso->triangles[j][i];
      v[j] = iso->vertices[0][k], iso->vertices[1][k], iso->vertices[2][k];
    }
    sl::vector3f n = cross(v[0],v[1],v[2]);
    float norm = n.two_norm();
    if (norm > 1e-6) {
      n /= norm;
      for (int j=0; j<3; ++j) {
	int k = iso->triangles[j][i];
	iso->normals[0][k] += n[0];
	iso->normals[1][k] += n[1];
	iso->normals[2][k] += n[2];
      }
    }
  }

  // Normalize normals
  for (int k=0; k<iso->nVertices; ++k) {
    sl::vector3f n;
    n = iso->normals[0][k], iso->normals[1][k], iso->normals[2][k];
    n = n.ok_normalized();
    iso->normals[0][k] = n[0];
    iso->normals[1][k] = n[1];
    iso->normals[2][k] = n[2];
  }  
}

void bsHQIsoBrainViewer::iso_relax() {
  mc_isosurface* iso = m_iso_geometry->iso();

  const int   relax_count = 8;
  const float relax_alpha = 0.7f / (float)(relax_count>0 ? relax_count : 1.0f);

  //  const float relax_alpha = 0.5f / (float)(relax_count>0 ? relax_count : 1.0f);

  std::vector< sl::vector3f > delta;
  delta.resize(iso->nVertices);

  for (int relax_cycles = 0; relax_cycles < relax_count; ++relax_cycles) {
    // Zero-out deltas
    for (int k=0; k<iso->nVertices; ++k) {
      delta[k].to_zero();
    }

    // Accumulate deltas
    for (int i=0; i<iso->nTriangles; ++i) {
      sl::point3f v[3];
      for (int j=0; j<3; ++j) {
	int k = iso->triangles[j][i];
	v[j] = iso->vertices[0][k], iso->vertices[1][k], iso->vertices[2][k];
      }
      
      for (int j=0; j<3; ++j) {
	int k = iso->triangles[j][i];
	delta[k] += 0.5f * ((v[(j+1)%3] - v[j]) + (v[(j+2)%3] - v[j]));
      }
    }
    
    // Apply deltas
    for (int k=0; k<iso->nVertices; ++k) {
      sl::point3f v;
      v = iso->vertices[0][k], iso->vertices[1][k], iso->vertices[2][k];
      
      v += relax_alpha * delta[k];
      
      iso->vertices[0][k] = v[0];
      iso->vertices[1][k] = v[1];
      iso->vertices[2][k] = v[2];
    }  
  }
}

void bsHQIsoBrainViewer::iso_render() {
  // Loop on triangles of iso i
  mc_isosurface* iso = m_iso_geometry->iso();

  GLboolean is_stencil = glIsEnabled(GL_STENCIL_TEST);

  glFrontFace(GL_CW);
  glBegin(GL_TRIANGLES); {
    //HACK il colore dovrebbe essere preso dal Brain come per le regioni
    glColor3f(0.5f, 0.5f, 0.4f);
    int code = -1;

    for (int i=0; i<iso->nTriangles; i++) {
	// NOTE: ordering is clockwise, normals towards interior!
	for (int d=0; d<3; d++) {
	  int k = iso->triangles[d][i];
	  if (!is_stencil) {
	    if (m_enable_subregions) {
	      int new_code = m_iso_geometry->vertex_code(k);
	      if (new_code != code) {
		code = new_code;
		if (code<0) {
		  glColor3f(0.5f, 0.5f, 0.4f);
		} else {
		  glColor3fv(m_brain->regionColorItem(code).to_pointer());
		}
	      }
	    }
	    glNormal3f(-iso->normals[0][k], -iso->normals[1][k], -iso->normals[2][k]);
	  }
	  glVertex3f(iso->vertices[0][k], iso->vertices[1][k], iso->vertices[2][k]);
	}
    }
  } glEnd();
      
  glFrontFace(GL_CCW);

#ifndef NDEBUG
  std::cerr << "Rendered iso for geometry: " << 
    iso->nVertices << " verts " << 
    iso->nTriangles << " tris " <<
    std::endl;
#endif
}


void bsHQIsoBrainViewer::section_draw_coded_red(int section_i) {

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
  for (int i=0; i<section->cortexRegionCount(); i++) {
    const bsCortexRegion *cort_reg = section->cortexRegionItem(i);
    render_boundaries(cort_reg);
  }

}

void bsHQIsoBrainViewer::render_boundaries(const bsCortexRegion* cr) {
  glColor3f(0.0f,0.0f,0.0f);
  glLineWidth(1.0f);
  glBegin(GL_LINE_LOOP);
  {
    int n_ext_vert = cr->externalBoundaryVertexCount();
    for(int i=0; i<n_ext_vert; i++) {
      glVertex2fv(cr->externalBoundaryVertexItem(i).to_pointer());
    }
  }
  glEnd();
  glBegin(GL_LINE_LOOP);
  {
    int n_int_vert = cr->internalBoundaryVertexCount();
    for(int i=0; i<n_int_vert; i++) {
      glVertex2fv(cr->internalBoundaryVertexItem(i).to_pointer());
    }
  }
  glEnd();
  glLineWidth(1.0f);
}

void bsHQIsoBrainViewer::render_region(const bsRegion* cr, int region_i) {
#if TRI_RENDER
  // triangle filling
  triangle_render(cr, region_i);
#endif // TRI_RENDER
}


void bsHQIsoBrainViewer::triangle_render(const bsRegion* cr, int region_i) {
  int n_tri = cr->triangleCount();
  glColor3ubv(region_index_to_color(region_i).to_pointer());

  glBegin(GL_TRIANGLES);
  {
    for(int i=0; i<n_tri; i++) {
      glVertex2fv(cr->triangleVertexItem(i,0).to_pointer());
      glVertex2fv(cr->triangleVertexItem(i,1).to_pointer());
      glVertex2fv(cr->triangleVertexItem(i,2).to_pointer());
    }
  }
  glEnd();
}




