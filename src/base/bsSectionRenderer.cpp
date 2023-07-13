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
#include "bsSectionRenderer.h"
#include "bsSection.h"
#include "bsCortexRegion.h"
#include "bsSubRegion.h"

#include <GL/gl.h>

#include <stdlib.h>

// for random neuron shift
//static double drand48() {
//  return rand()/(double)RAND_MAX;
//}

bsSectionRenderer::bsSectionRenderer() {
  m_fill_rendering = false;
  m_lighted_rendering = false;
  m_stripe_rendering = false;
  m_step_size = 1;
}
    
bsSectionRenderer::~bsSectionRenderer() {
}

// render

void bsSectionRenderer::render(const bsSection* s) {
  assert(s);
  //  assert(subRegionColorCount() > s->maxSubRegionId());
  //  assert(neuronTypeColorCount() > s->maxNeuronType());

  GLboolean lighted;
  glGetBooleanv(GL_LIGHTING, &lighted);

  m_lighted_rendering = (lighted != GL_FALSE);
  if (m_lighted_rendering) {
    glEnable(GL_NORMALIZE);
  }

  // Boundaries
  for (int i=0; i<s->cortexRegionCount(); i++) {
    const bsCortexRegion* r = s->cortexRegionItem(i);
    if (enableSubRegions() && (r->subRegionCount() > 0)) {
      for (int j=0; j<r->subRegionCount(); j++) {
	renderSubRegion(r->subRegionItem(j));
      }
    } else {
      if (enableExternal()) {
	renderBoundary(r);
      }
      if (enableInternal()) {
	for (int j=0; j<r->holeCount(); j++) {
	  renderHole(r, j);
	}
      }
    }
  }
  // The rendering is done after the boundaries
  // Neurons
  //if (enableNeuronTypes()) {
  //renderNeurons(s);
  //}

  if (m_lighted_rendering) {
    glDisable(GL_NORMALIZE);
  }

}

void bsSectionRenderer::renderNeurons(const bsSection* s) {
  assert(s);
  assert(neuronTypeColorCount() > s->maxNeuronType());
  if ( !s->ghostSection() ) {
    if (m_lighted_rendering) {
      glDisable(GL_LIGHTING);
    }
    
    srand(1000000);
    
    int ntp = s->maxNeuronType()+1;
    for (int tp=0; tp<ntp; tp++) {
      if (neuronTypeEnabled(tp)) {
	if (isNeuronDimensionIndependent()) {
	  setNeuronSize(neuronTypeDimension(tp));
	}
	glBegin(GL_POINTS);
	glColor3fv(neuronTypeColor(tp).to_pointer());
	int ni = s->neuronCount(tp);
	for (int i=0; i<ni; i++) {
	  sl::point3f n = s->neuronItem3(tp,i);
	  if (!neuronTypeAligned(tp)) {  
	    sl::column_vector3f v_rand;
	    if (drand48() >  0.5) {
	      v_rand = 0.0f, 0.0f,   drand48() * s->getNeuronUpThickness();
	    } else {
	      v_rand = 0.0f, 0.0f, - drand48() * s->getNeuronDownThickness();
	    }
	    glVertex3fv((n+v_rand).to_pointer());
	  } else {
	    glVertex3fv(n.to_pointer());
	  }
	}
	glEnd();
      }
    }
    if (m_lighted_rendering) {
      glEnable(GL_LIGHTING);
    }
  }
}

void bsSectionRenderer::renderVertex(const sl::column_vector3f& vdown,
				     const sl::column_vector3f& vup,
				     const sl::point3f& pA, 
				     const sl::point3f& p, 
				     const sl::point3f& pB) {

  static const float SHARP_EPSILON = sl::sqr(std::cos(150.0f * 3.14f/180.0f));

  if (!m_lighted_rendering) {
    if (stripeRendering()) {
      sl::point3f pvup; pvup = p[0]+vup[0],p[1]+vup[1],p[2]+vup[2];
      sl::point3f pvdown; pvdown = p[0]+vdown[0],p[1]+vdown[1],p[2]+vdown[2];
      glVertex3fv(pvup.to_pointer());
      glVertex3fv(pvdown.to_pointer());
    } else {
      glVertex3fv(p.to_pointer());
    }
  } else {
    bool sharp = false;
    sl::column_vector3f n_left; n_left = 0.0f, 0.0f, 0.0f;
    sl::column_vector3f n_right; n_right = 0.0f, 0.0f, 0.0f;
    sl::column_vector3f tang = pB-pA;
    sl::column_vector3f v1 = pA - p;
    sl::column_vector3f v2 = pB - p;
    sharp = (sl::sqr(v1.dot(v2)) < SHARP_EPSILON*v1.two_norm_squared()*v2.two_norm_squared());
    if (!sharp) {
      n_left = sl::column_vector3f(tang[1],-tang[0],0.0f);
    } else {
      n_left = sl::column_vector3f(-v1[1],v1[0],0.0f);
      n_right = sl::column_vector3f(v2[1],-v2[0],0.0f);
    }
  
    if (stripeRendering()) {
      sl::point3f pvup; pvup = p[0]+vup[0],p[1]+vup[1],p[2]+vup[2];
      sl::point3f pvdown; pvdown = p[0]+vdown[0],p[1]+vdown[1],p[2]+vdown[2];
      glNormal3fv(n_left.to_pointer());
      glVertex3fv(pvup.to_pointer());
      glVertex3fv(pvdown.to_pointer());
      if (sharp) {
	glNormal3fv(n_right.to_pointer());
	glVertex3fv(pvup.to_pointer());
	glVertex3fv(pvdown.to_pointer());
      } 
    } else {
      glNormal3fv(n_left.to_pointer());
      glVertex3fv(p.to_pointer());
      if (sharp) {
	glNormal3fv(n_right.to_pointer());
	glVertex3fv(p.to_pointer());
      } 
    }
  }
}


void bsSectionRenderer::renderBoundary(const bsCortexRegion* s) {
  assert(s);
  glColor3fv(externalColor().to_pointer());

  // Draw boundary
  if (!fillRendering() || stripeRendering()) {  
    glBegin(stripeRendering() ? GL_TRIANGLE_STRIP : GL_LINE_STRIP);
    int n = s->externalBoundaryVertexCount();
    bool exit=false;
    for (int i=0; i<=n && !exit; i=sl::min(i+stepSize(),n)) {
      exit = (i==n);
      sl::column_vector3f vup; vup = 0.0f, 0.0f, s->getParentSection()->getUpThickness();
      sl::column_vector3f vdown; vdown = 0.0f, 0.0f, - s->getParentSection()->getDownThickness();
      sl::point3f pA = s->externalBoundaryVertexItem3((i-1+n)%n);
      sl::point3f p = s->externalBoundaryVertexItem3(i%n);
      sl::point3f pB = s->externalBoundaryVertexItem3((i+1)%n);
      renderVertex(vdown,vup,pA,p,pB);
    } glEnd();
  }

  // Draw interior
  if (fillRendering()) {
    sl::column_vector3f vup; 
    sl::column_vector3f vdown; 
    if (stripeRendering()) {
      vup = 0.0f, 0.0f, s->getParentSection()->getUpThickness();
      vdown = 0.0f, 0.0f, - s->getParentSection()->getDownThickness();
    }
    int n_tri = s->triangleCount();
    glBegin(GL_TRIANGLES); {
      if (m_lighted_rendering) {
	// normal up; CCW
	glNormal3f(0.0f,0.0f,1.0f);
      }
      for(int i=0; i<n_tri; i++) {
	glVertex3fv((s->triangleVertexItem3(i,0)+vup).to_pointer());
	glVertex3fv((s->triangleVertexItem3(i,1)+vup).to_pointer());
	glVertex3fv((s->triangleVertexItem3(i,2)+vup).to_pointer());
      }
      if (stripeRendering()) {
	if (m_lighted_rendering) {
	  // normal down; CW
	  glNormal3f(0.0f,0.0f,-1.0f);
	}
	for (int i=0; i<n_tri; i++) {
	  glVertex3fv((s->triangleVertexItem3(i,0)+vdown).to_pointer());
	  glVertex3fv((s->triangleVertexItem3(i,1)+vdown).to_pointer());
	  glVertex3fv((s->triangleVertexItem3(i,2)+vdown).to_pointer());
	}
      }
    } glEnd();
  }
}

void bsSectionRenderer::renderHole(const bsCortexRegion* s, int nh) {
  assert(s);
  
  glBegin(stripeRendering() ? GL_TRIANGLE_STRIP : GL_LINE_STRIP);
  glColor3fv(internalColor().to_pointer());
  int n = s->holeVertexCount(nh);

  bool exit=false;
  for (int i=0; i<=n && !exit; i=sl::min(i+stepSize(),n)) {
    exit = (i==n);
    sl::column_vector3f vup; vup = 0.0f, 0.0f, s->getParentSection()->getUpThickness();
    sl::column_vector3f vdown; vdown = 0.0f, 0.0f, - s->getParentSection()->getDownThickness();
    sl::point3f pA = s->holeVertexItem3(nh,(i-1+n)%n);
    sl::point3f p = s->holeVertexItem3(nh,i%n);
    sl::point3f pB = s->holeVertexItem3(nh,(i+1)%n);
    renderVertex(vdown,vup,pA,p,pB);
  }  
  glEnd();
}

void bsSectionRenderer::renderSubRegion(const bsSubRegion* s) {
  assert(s);
  assert(subRegionColorCount() > s->getId());
  assert(s);

  glColor3fv(subRegionColor(s->getId()).to_pointer());

  // Draw boundary
  if (!fillRendering() || stripeRendering()) {
    glBegin(stripeRendering() ? GL_TRIANGLE_STRIP : GL_LINE_STRIP);
    int n = s->externalBoundaryVertexCount();
    bool exit=false;
    for (int i=0; i<=n && !exit; i=sl::min(i+stepSize(),n)) {
      exit = (i==n);
      sl::column_vector3f vup; vup = 0.0f, 0.0f, s->getParentSection()->getUpThickness();
      sl::column_vector3f vdown; vdown = 0.0f, 0.0f, - s->getParentSection()->getDownThickness();
      sl::point3f pA = s->externalBoundaryVertexItem3((i-1+n)%n);
      sl::point3f p = s->externalBoundaryVertexItem3(i%n);
      sl::point3f pB = s->externalBoundaryVertexItem3((i+1)%n);
      renderVertex(vdown,vup,pA,p,pB);
    } glEnd();
  }
  
  // Draw interior
  if (fillRendering()) {
    sl::column_vector3f vup; 
    sl::column_vector3f vdown; 
    if (stripeRendering()) {
      vup = 0.0f, 0.0f, s->getParentSection()->getUpThickness();
      vdown = 0.0f, 0.0f, - s->getParentSection()->getDownThickness();
    }
    int n_tri = s->triangleCount();

    glBegin(GL_TRIANGLES); {
      if (m_lighted_rendering) {
	// normal up; CCW
	glNormal3f(0.0f,0.0f,1.0f);
      }
      for(int i=0; i<n_tri; i++) {
	glVertex3fv((s->triangleVertexItem3(i,0)+vup).to_pointer());
	glVertex3fv((s->triangleVertexItem3(i,1)+vup).to_pointer());
	glVertex3fv((s->triangleVertexItem3(i,2)+vup).to_pointer());
      }
      if (stripeRendering()) {
	if (m_lighted_rendering) {
	  // normal down; CW
	  glNormal3f(0.0f,0.0f,-1.0f);
	}
	for (int i=0; i<n_tri; i++) {
	  glVertex3fv((s->triangleVertexItem3(i,0)+vdown).to_pointer());
	  glVertex3fv((s->triangleVertexItem3(i,1)+vdown).to_pointer());
	  glVertex3fv((s->triangleVertexItem3(i,2)+vdown).to_pointer());
	}
      }
    } glEnd();
  }
}

void bsSectionRenderer::setNeuronSize(float neuronsize) {
    glPointSize(neuronsize < 1.0f ? 1.0f : 
		(neuronsize > 20 ? 20 : neuronsize));
}

void bsSectionRenderer::setBorderSize(float bordersize) {
    glLineWidth(bordersize < 1.0f ? 1.0f : 
		(bordersize > 20 ? 20 : bordersize));
}


