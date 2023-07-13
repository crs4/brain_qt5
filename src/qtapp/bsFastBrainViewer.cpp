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
#include "bsFastBrainViewer.h"
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

bsFastBrainViewer::bsFastBrainViewer(QWidget* parent) 
    : bsBrainViewer(parent) {
    m_selected_render=new bsSectionRenderer();
    m_selected_renderer_index = -1;
    m_enable_selected_renderer = false;
    m_draft_quality = false;
}


bsFastBrainViewer::~bsFastBrainViewer() {
}


// Slots

void bsFastBrainViewer::handleSectionChange(int){
  updateGL();
}

void bsFastBrainViewer::setEnableSubRegions(bool b) {
  m_render->setEnableSubRegions(b);
  m_selected_render->setEnableSubRegions(b);
  updateGL();
}

void bsFastBrainViewer::setEnableNeuronTypes(bool b) {
  m_render->setEnableNeuronTypes(b);
  m_selected_render->setEnableNeuronTypes(b);
  updateGL();
}

void bsFastBrainViewer::setEnableSelectedRenderer(bool b) {
  m_enable_selected_renderer=b;
}

void bsFastBrainViewer::setStepSize(int value) {
  m_render->setStepSize(value);
  m_selected_render->setStepSize(value);
  updateGL();
}

void bsFastBrainViewer::setDraftQuality(bool b) {
  if (b) {
    m_render->setStepSize(4);
    m_selected_render->setStepSize(4);
  } else {
    m_render->setStepSize(1);
    m_selected_render->setStepSize(1);
  }
  m_draft_quality = b;
  updateGL();
}


void bsFastBrainViewer::handleBrainChange() {

  bsSectionedBrain* b = m_document->getBrain2D();
  if (b && b->sectionCount() >0) {
    m_render->resizeSubRegionColors(b->maxSubRegionId()+1);
    m_render->resizeNeuronTypeColors(b->maxNeuronType()+1);
    m_render->setStripeRendering(true);
    m_render->setFillRendering(true);

    m_selected_render->resizeSubRegionColors(b->maxSubRegionId()+1);
    m_selected_render->resizeNeuronTypeColors(b->maxNeuronType()+1);
    m_selected_render->setStripeRendering(true);
    m_selected_render->setFillRendering(true);

    sl::color3f c;
    c= 1.0f, 0.0f, 0.0f;
    m_selected_render->setExternalColor(c);
    m_selected_render->setInternalColor(c);
    {
      for (int i=0; i<m_selected_render->subRegionColorCount(); i++) {
	m_selected_render->setSubRegionColor(i,c);
      }
    }
    {
      for (int i=0; i<m_selected_render->neuronTypeColorCount(); i++) {
	m_selected_render->setNeuronTypeColor(i,c);
      }
    }

    if(m_enable_selected_renderer) {
      m_selected_renderer_index=0;
    }
    sl::aabox3f v_box = b->boundingBox();
    setInitBox(v_box);

    reset();
    setDraftQuality(m_draft_quality);
  }
}


void bsFastBrainViewer::setSelectedRendererIndex(int n) {
  bsSectionedBrain* b = m_document->getBrain2D();
  if (b && b->goodSectionIndex(n)) {
    m_selected_renderer_index = n;
  } else {
    m_selected_renderer_index = -1;
  }  
  updateGL();
}



// command -- rendering

void bsFastBrainViewer::localPaintGL(){

  // Draw FastBrainViewer

  bsSectionedBrain* brain2D = m_document->getBrain2D();
  if (brain2D) {
    m_render->setBorderSize(2.0f);

    glPushMatrix(); {
      for (int n_sect=0; n_sect < brain2D->sectionCount(); n_sect++) {
	const bsSection* section = brain2D->sectionItem(n_sect);
	if (m_enable_selected_renderer && n_sect == m_selected_renderer_index) {
	  m_selected_render->render(section);
	} else {
	  m_render->render(section);
	}
      }
    } glPopMatrix();

    m_render->setBorderSize(1.0f);
  }
}
  
