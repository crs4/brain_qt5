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
#include "bsBrainViewer.h"
#include "bsSectionedBrain.h"
#include "bsSectionRenderer.h"
#include "bsNeuronRenderer.h"

#include "qtCommandHistory.h"
#include "bsCmdCurrentSection.h"
#include "bsCmdUpDownSection.h"
#include "bsDocument.h"
#include <iostream>
#include <math.h>

#include <sl/axis_aligned_box.hpp>


bsBrainViewer::bsBrainViewer(QWidget* parent) 
  : bsViewer3D(parent) {
  m_document=NULL;
  m_render=new bsSectionRenderer();

  m_extinction_zeta = 0.01f;
  m_extinction_zeta_threshold[0] = 0;
  m_extinction_zeta_threshold[1] = 2000;

  m_cutting_planes_flag = false;
  m_cutting_caps_flag = false;
  m_cutting_tool_flag = false;
  m_light_two_side = false;
  resetCuttingBox();
}


bsBrainViewer::~bsBrainViewer() {
#ifndef NDEBUG
  std::cerr << "#################### Destroying bsBrainViewer" << std::endl;
#endif  
  if(m_render) delete m_render;
  m_render=NULL;
}

// Slots


void bsBrainViewer::setDocument(bsDocument* doc){
    assert(doc);
    m_document=doc;
    //    handleBrainChange();
    connect(m_document,SIGNAL(newBrain2D(bool)),this,SLOT(handleBrainChange()));
    connect(m_document,SIGNAL(newBrain2D(bool)),this,SLOT(resetCuttingBox()));
    connect(m_document,SIGNAL(newBrain2D(bool)),this,SIGNAL(newBrain2D(bool)));
    connect(m_document,SIGNAL(brain2DChanged(int)),this,SLOT(handleSectionChange(int)));
    connect(m_document,SIGNAL(regionOptionsChanged()),this,SLOT(setupRegionOptions()));
    connect(m_document,SIGNAL(neuronTypeOptionsChanged()),this,SLOT(setupNeuronTypeOptions()));
}


void bsBrainViewer::resetCuttingBox() {
  sl::point3f pmin; pmin = -1.0f, -1.0f, -1.0f;
  sl::point3f pmax; pmax = 1.0f, 1.0f, 1.0f;
  sl::obox3f cbox = sl::obox3f(sl::aabox3f(pmin,pmax));
  if (m_document) {
    bsSectionedBrain* brain2D = m_document->getBrain2D();
    if (brain2D != NULL) {
      cbox = sl::obox3f(brain2D->boundingBox());
    } 
  }
  setCuttingBox(cbox);
  updateGL();
}

sl::aabox3f bsBrainViewer::getBoundingBox() const{
  sl::aabox3f result;
  if (m_document) {
    bsSectionedBrain* brain2D = m_document->getBrain2D();
    if (brain2D != NULL) {
      result=brain2D->boundingBox();
    } 
  }
  return result;
}

void bsBrainViewer::setCuttingBox(const sl::obox3f& box) {
  m_cutting_box = box;
  emit cuttingPlanesChanged(m_cutting_box);
}

// Queries

bsDocument* bsBrainViewer::getDocument() const {

  return m_document;
}


bsSectionRenderer* bsBrainViewer::getRenderer() const {

  return m_render;
}

void bsBrainViewer::paintGL() {
  if (!isVisible()) return;
  glClearColor(m_bkg_color[0],m_bkg_color[1],m_bkg_color[2], 1.0f ); 
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glColor3f(1.0, 0.0, 0.0);

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

  // Enabling bidirectional light 
  if (lightTwoSideEnabled()) {
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
  } else {
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
  }

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, 
		  GL_AMBIENT_AND_DIFFUSE);

  {
    sl::color4f c; 
    c=0.0f,0.0f,0.0f,1.0f; // c=0.1f, 0.1f, 0.1f, 1.0f;
    glLightfv(GL_LIGHT0, GL_AMBIENT, c.to_pointer());
    c=1.0f, 1.0f, 1.0f, 1.0f;
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  c.to_pointer());
    c=0.0f,0.0f,0.0f,1.0f; // c=0.1f, 0.1f, 0.1f, 1.0f;
    glLightfv(GL_LIGHT0, GL_SPECULAR, c.to_pointer());
  }

  glMatrixMode( GL_PROJECTION );
  glLoadMatrixf( cameraAP().to_pointer() );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  {
    sl::vector4f v;
    v=0.0f, 0.0f, 1.0f, 0.0f;
    glLightfv(GL_LIGHT0, GL_POSITION, v.to_pointer());
  }

  glLoadMatrixf( cameraV().to_pointer() );
  
  if (cuttingPlanesEnabled() && fancyRenderingEnabled()) {
    cutPaintGL();
    // neuron rendering is done after boundary rendering
    if (m_render->enableNeuronTypes()) {
      enableCutPlanesExcept(-1);
      glDisable(GL_LIGHTING);
      bsNeuronRenderer neuron_render(this);
      neuron_render.qualityNeuronRendering();
      for (int i=0; i<6; i++) {
	glDisable(GL_CLIP_PLANE0+i);
      }	
    }
  } else {
    localPaintGL();
    // neuron rendering is done after boundary rendering
    if (m_render->enableNeuronTypes()) {
      glDisable(GL_LIGHTING);
      bsNeuronRenderer neuron_render(this);
      neuron_render.qualityNeuronRendering();
    }
    if((!fancyRenderingEnabled() && cuttingPlanesEnabled()) || cuttingToolEnabled()) {
      glDisable(GL_LIGHTING);
      drawCuttingBox();
    }
  }    

  // Reset
  glShadeModel(GL_FLAT);
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  paintPainters();

}

void bsBrainViewer::cutPaintGL() {
  if (m_document) {

    enableCutPlanesExcept(-1);

    localPaintGL();
    
    for (int i=0; i<6; i++) {
      glDisable(GL_CLIP_PLANE0+i);
    }
	
    if (cuttingCapsEnabled()) {
      capsPaintGL();
    }
  } 
}

void bsBrainViewer::enableCutPlanesExcept(int j) {
  if (m_document) {
    bsSectionedBrain* brain2D = m_document->getBrain2D();
    if (brain2D != NULL) {
      sl::aabox3f bb = brain2D->boundingBox();
      sl::obox3f  cb = getCuttingBox();
      for (int i=0; i<6; i++) {
	sl::plane3f cp = cb.plane(i);
	if (i==j || bb.is_fully_below(cp)) {
	  glDisable(GL_CLIP_PLANE0+i);
	} else {
	  GLdouble equation[4] = { -cp[0], -cp[1], -cp[2], -cp[3] }; 
	  glClipPlane(GL_CLIP_PLANE0+i, equation);
	  glEnable(GL_CLIP_PLANE0+i);
	}    
      }
    }
  }
}


void bsBrainViewer::drawPlane(int i) {
  sl::obox3f  cb = getCuttingBox();
  sl::plane3f cp = cb.plane(i);
  
  const float val = 1.4f;
  sl::color3f bkg_color = m_render->externalColor();
  bkg_color[0] = bkg_color[0] * val <= 1.0f ? bkg_color[0] * val : 1.0f;
  bkg_color[1] = bkg_color[1] * val <= 1.0f ? bkg_color[1] * val : 1.0f;
  bkg_color[2] = bkg_color[2] * val <= 1.0f ? bkg_color[2] * val : 1.0f;

  glColor3f(bkg_color[0],bkg_color[1],bkg_color[2]);
  glBegin(GL_QUADS); {
    sl::point3f c0;
    sl::point3f c1;
    sl::point3f c2;
    sl::point3f c3;
    switch (i) {
    case 0: 
      {
	c0 = cb.corner(0);
	c1 = cb.corner(4);
	c2 = cb.corner(6);
	c3 = cb.corner(2);
      }
      break;
    case 1: 
      {
	c0 = cb.corner(1);
	c1 = cb.corner(3);
	c2 = cb.corner(7);
	c3 = cb.corner(5);
      }
      break;
    case 2: 
      {
	c0 = cb.corner(0);
	c1 = cb.corner(1);
	c2 = cb.corner(5);
	c3 = cb.corner(4);
      }
      break;
    case 3: 
      {
	c0 = cb.corner(2);
	c1 = cb.corner(6);
	c2 = cb.corner(7);
	c3 = cb.corner(3);
      }
      break;
    case 4: 
      {
	c0 = cb.corner(0);
	c1 = cb.corner(2);
	c2 = cb.corner(3);
	c3 = cb.corner(1);
      }
      break;
    case 5: 
      {
	c0 = cb.corner(4);
	c1 = cb.corner(5);
	c2 = cb.corner(7);
	c3 = cb.corner(6);
      }
      break;
    }
    
    glNormal3fv(cp.normal().to_pointer());
    glVertex3fv(c0.to_pointer());
    glVertex3fv(c1.to_pointer());
    glVertex3fv(c2.to_pointer());
    glVertex3fv(c3.to_pointer());
  } glEnd();
}


void bsBrainViewer::capsPaintGL() {
  if (m_document) {
    bsSectionedBrain* brain2D = m_document->getBrain2D();
    if (brain2D != NULL) {
      sl::point3f eye = cameraV().inverse() * sl::point3f::zero();

      sl::aabox3f bb = brain2D->boundingBox();
      sl::obox3f  cb = getCuttingBox();
      for (int i=0; i<6; i++) {
	sl::plane3f cp = cb.plane(i);
	if (bb.is_fully_below(cp)) {
	  // So nothing
	} else if (cp.value(eye) <= 0.0f) {
	  // HACK!!! Original test is 
	  // } else if (transformation(cameraV(), cp.normal())[2] < 0.0f) {

	  // Not facing the camera, do nothing
	} else {
	  // Draw cap for plane i 

	  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	  glDepthMask(GL_FALSE);
	  glClearStencil(0x00);
	  glClear(GL_STENCIL_BUFFER_BIT);

	  GLdouble equation[4] = { cp[0], cp[1], cp[2], cp[3] }; 
	  glClipPlane(GL_CLIP_PLANE0+i, equation);
	  glEnable(GL_CLIP_PLANE0+i);

	  glStencilFunc(GL_ALWAYS, 0, 0x01);
	  glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
	  glEnable(GL_STENCIL_TEST);

	  glDisable(GL_DEPTH_TEST);
	  glDisable(GL_CULL_FACE);
	  localPaintGL();

	  // Stencil contains 1 iff plane is inside brain

	  enableCutPlanesExcept(i);

	  glStencilFunc(GL_NOTEQUAL, 0x00, 0xff);
	  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	  glDepthMask(GL_TRUE);
	  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	  glEnable(GL_DEPTH_TEST);
	  drawPlane(i);

	  glDisable(GL_STENCIL_TEST);

	  for (int j=0; j<6; j++) {
	    glDisable(GL_CLIP_PLANE0+j);
	  }	

	}
      }
    }
  }
}

float bsBrainViewer::getExtinctionZetaValue() const {
  return m_extinction_zeta;
}

float bsBrainViewer::getExtinctionZetaThresholdStart() const {
  return (float)m_extinction_zeta_threshold[0];
}

float bsBrainViewer::getExtinctionZetaThresholdEnd() const {
  return (float)m_extinction_zeta_threshold[1];
}

void bsBrainViewer::setupRegionOptions() {
  bsSectionedBrain* b = m_document->getBrain2D();
  if (b) {
    m_render->resizeSubRegionColors(b->regionLabelCount());
    for(int i = 0; i < m_render->subRegionColorCount(); i++) {
      m_render->setSubRegionColor(i,b->regionColorItem(i));
    }
  }
}

void bsBrainViewer::setupNeuronTypeOptions() {
  bsSectionedBrain* b = m_document->getBrain2D();
  if (b) {
    m_render->resizeNeuronTypeColors(b->neuronTypeLabelCount());
    for(int i = 0; i < m_render->neuronTypeColorCount(); i++) {
      m_render->setNeuronTypeColor(i,b->neuronTypeColorItem(i));
      m_render->setNeuronTypeAligned(i,b->neuronTypeAligned(i));
      m_render->setNeuronTypeEnabled(i,b->neuronTypeEnabled(i));
    }
  }
}


void bsBrainViewer::handleExtinctionZetaChange(int value) {
  if (value <=0) {
    value=1;
  } else if (value > 100) {
    value=100;
  }
  m_extinction_zeta = (float)value/100.0f;
  updateGL();
}


void bsBrainViewer::handleExtinctionZetaThresholdChangeStart(int value) {
  m_extinction_zeta_threshold[0] = value;
  updateGL();
}

void bsBrainViewer::handleExtinctionZetaThresholdChangeEnd(int value) {
  m_extinction_zeta_threshold[1] = value;
  updateGL();
}


void bsBrainViewer::setCuttingPlanesFlag(bool b) {
  m_cutting_planes_flag = b;
  updateGL();
}

void bsBrainViewer::setCuttingCapsFlag(bool b) {
  m_cutting_caps_flag = b;
  updateGL();
}

void bsBrainViewer::setLightTwoSide(bool b) {
  m_light_two_side = b;
  updateGL();
}


void bsBrainViewer::drawCuttingBox() {
  sl::obox3f cb = getCuttingBox();
  
  glColor3f(1.0, 0.0, 0.0);
  sl::point3f cbN;

  glBegin(GL_LINE_LOOP); {
    cbN = cb.corner(0);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(1);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(3);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(2);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
  } glEnd();
 
  glBegin(GL_LINE_LOOP); {
    cbN = cb.corner(6);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(4);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(5);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

    cbN = cb.corner(7);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

  } glEnd();

  glBegin(GL_LINE_LOOP); {
    cbN = cb.corner(1);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(3);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(7);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

    cbN = cb.corner(5);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

  } glEnd();

  glBegin(GL_LINE_LOOP); {
    cbN = cb.corner(0);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(4);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(6);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

    cbN = cb.corner(2);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

  } glEnd();

  glBegin(GL_LINE_LOOP); {
    cbN = cb.corner(2);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(6);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(7);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

    cbN = cb.corner(3);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

  } glEnd();

  glBegin(GL_LINE_LOOP); {
    cbN = cb.corner(0);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(1);
    glVertex3f(cbN[0], cbN[1], cbN[2]);
    
    cbN = cb.corner(5);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

    cbN = cb.corner(4);
    glVertex3f(cbN[0], cbN[1], cbN[2]);

  } glEnd();

}


void bsBrainViewer::setEnableNeuronTypes(bool b) {
  m_render->setEnableNeuronTypes(b);
  updateGL();
}

