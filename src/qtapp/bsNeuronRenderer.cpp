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
#include "bsSectionedBrain.h"
#include "bsSectionRenderer.h"
#include "bsNeuronRenderer.h"
#include "bsBrainViewer.h"

#include "bsDocument.h"
#include <iostream>
#include <math.h>

#include <sl/axis_aligned_box.hpp>

bsNeuronRenderer::bsNeuronRenderer(const bsBrainViewer* viewer) {
  m_viewer = viewer;
  m_zeta_extinction_threshold[0] = m_viewer->getExtinctionZetaThresholdStart();
  m_zeta_extinction_threshold[1] = m_viewer->getExtinctionZetaThresholdEnd();
}


bsNeuronRenderer::~bsNeuronRenderer() {
    m_viewer=NULL;
}



void bsNeuronRenderer::qualityNeuronRendering() {

  bsSectionedBrain* brain2D = m_viewer->getDocument()->getBrain2D();

  if (brain2D) {
    glPushAttrib(GL_ALL_ATTRIB_BITS); {

      sl::aabox3f scene_box = brain2D->boundingBox();

      float extinction_zeta = (float)(m_viewer->getExtinctionZetaValue());
      
      setZetaExtinctionThreshold(m_viewer->getExtinctionZetaThresholdStart(),
				 m_viewer->getExtinctionZetaThresholdEnd());
      

      static std::vector< sl_color3ub >    *rgb_shell            = new std::vector< sl_color3ub >;
      static std::vector< unsigned short > *z_shell              = new std::vector< unsigned short >;
      static std::vector< sl_color3ub >    *rgb_neuron_and_shell = new std::vector< sl_color3ub >;
      static std::vector< unsigned short > *z_neuron_and_shell   = new std::vector< unsigned short >;
  
      int W = m_viewer->width();
      int H = m_viewer->height();

      if ((int)rgb_shell->size() != W*H) {
	rgb_shell->resize(W*H);
	z_shell->resize(W*H);
	rgb_neuron_and_shell->resize(W*H);
	z_neuron_and_shell->resize(W*H);
      }

      glReadBuffer(GL_BACK_LEFT);
      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, &((*rgb_shell)[0]));
      glReadPixels(0, 0, W, H, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, &((*z_shell)[0]));

      glDisable(GL_LIGHTING);
      glClear(GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);

      glPointSize(1);
      renderNeurons();
      glPointSize(1);

      glReadBuffer(GL_BACK_LEFT);
      glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, &((*rgb_neuron_and_shell)[0]));
      glReadPixels(0, 0, W, H, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, &((*z_neuron_and_shell)[0]));

      float tau = -log(extinction_zeta)/m_zeta_extinction_threshold[1];
      //      float tau = -log(0.01)/(extinction_zeta*(scene_box.diagonal().two_norm()));
      
      //      std::cerr << "tau = " << tau << "  z_ext = " << extinction_zeta << "  start = " 
      //	<< m_zeta_extinction_threshold[0] << " end = " 
      //	<< m_zeta_extinction_threshold[1] <<  std::endl; 


      updateNeuronColors(rgb_shell,
			 z_shell,
			 rgb_neuron_and_shell,
			 z_neuron_and_shell,
			 tau);

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();

      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);

      bool plane_enabled[6];
      for (int i=0; i<6; i++) {
	plane_enabled[i] = (bool)glIsEnabled(GL_CLIP_PLANE0+i);
	if (plane_enabled[i]) {
	  glDisable(GL_CLIP_PLANE0+i);
	}
      }

      glRasterPos3f(-1.,-1.,-1.);
      glDrawBuffer(GL_BACK_LEFT);
      glPixelZoom(1.0f,1.0f);
      glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, &((*rgb_neuron_and_shell)[0]));

      for (int i=0; i<6; i++) {
	if (plane_enabled[i]) {
	  glEnable(GL_CLIP_PLANE0+i);
	}
      }

      glPopMatrix();
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);

    } glPopAttrib();
  }
}


void bsNeuronRenderer::fastNeuronRendering() {

    glClear(GL_DEPTH_BUFFER_BIT);
    glPointSize(1);
    renderNeurons();
    glPointSize(1);

}


void bsNeuronRenderer::renderNeurons() {

  bsSectionedBrain* brain2D = m_viewer->getDocument()->getBrain2D();
  if (brain2D) {
    bsSectionRenderer *render = m_viewer->getRenderer(); 
    float neuron_size=m_viewer->getDocument()->getNeuronSize();
    if (neuron_size==0.0f) {
      sl::point3f p;
      p[0]=m_viewer->initBox().diagonal()[0];
      p[1]=m_viewer->initBox().diagonal()[1];
      p[2]=m_viewer->initBox().diagonal()[2];
      neuron_size= m_viewer->worldToNormalized(p)[0];
    }
    render->setNeuronSize(neuron_size);
    render->setNeuronTypeDimensions(brain2D->neuronTypeDimensions());
    glPushMatrix(); {
      for (int n_sect=0; n_sect < brain2D->sectionCount(); n_sect++) {
	const bsSection* section = brain2D->sectionItem(n_sect);
	render->renderNeurons(section);
      }
    } glPopMatrix();

    render->setNeuronSize(1.0f);
  }
}


void bsNeuronRenderer::updateNeuronColors(const std::vector<sl_color3ub>* rgb_shell,
					  const std::vector<unsigned short>* z_shell,
					  std::vector<sl_color3ub>* rgb_neuron_and_shell,
					  const std::vector<unsigned short>* z_neuron_and_shell,
					  float tau) {
  assert(rgb_shell);
  assert(z_shell);
  assert(rgb_neuron_and_shell);
  assert(z_neuron_and_shell);
  assert(rgb_shell->size() == z_shell->size());
  assert(rgb_neuron_and_shell->size() == z_neuron_and_shell->size());
  assert(rgb_shell->size() == rgb_neuron_and_shell->size());

  int n = rgb_shell->size();

  float z_near = m_viewer->cameraNear();
  float z_far  = m_viewer->cameraFar();
  float z_scale = (z_far - z_near) / 65535.0f;

  int k = 0;
  float sum_d = 0.;
  for (int i=0; i<n; i++) {
    sl_color3ub rgb_s = (*rgb_shell)[i];
    sl_color3ub rgb_n = (*rgb_neuron_and_shell)[i];
    if (! (rgb_s == rgb_n) ) {  // (! ==) al posto di (!=)
      k++;
      // Attenuation
      float d = z_scale * ((float)(*z_neuron_and_shell)[i] - (float)(*z_shell)[i]);
      //std::cerr << "d = " << d << std::endl;
      sum_d += d;
      //std::cerr << "average d = " << sum_d /(i+1) << std::endl; 
      if (d < 0) { d = 0.0; }

      float alpha = 0.0f;
      if (d > zetaExtinctionThresholdStart() && d < zetaExtinctionThresholdEnd()) {
	alpha = exp(-tau * d); // TODO: convert in hash table
      }
      (*rgb_neuron_and_shell)[i] = sl_color3ub((unsigned char)(rgb_s[0]*(1.0f-alpha) + rgb_n[0] * alpha),
  					       (unsigned char)(rgb_s[1]*(1.0f-alpha) + rgb_n[1] * alpha),
  					       (unsigned char)(rgb_s[2]*(1.0f-alpha) + rgb_n[2] * alpha));
#if TEST
      if (d>=0 && d<300) {
	(*rgb_neuron_and_shell)[i] = sl_color3ub(255,0,0);
      } else if (d>=300 && d<600) {
	(*rgb_neuron_and_shell)[i] = sl_color3ub(0,255,0);
      } else if (d>=600 && d<900) {
	(*rgb_neuron_and_shell)[i] = sl_color3ub(0,0,255);
      } else {
	(*rgb_neuron_and_shell)[i] = sl_color3ub(255,255,255);
      }
#endif
    }
  }
}


