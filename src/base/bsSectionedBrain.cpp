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
#include "bsSection.h"
#include "bsCortexRegion.h"
#include "bsSubRegion.h"
#include "bsGeometryHelpers.h"
#include <string.h>
#include <stdio.h>
#include <iostream>


// Creation & Destruction

bsSectionedBrain::bsSectionedBrain(){
  assert(invariant());
}

bsSectionedBrain::~bsSectionedBrain(){
  clear();
}

// Invariant

bool  bsSectionedBrain::invariant() const {
  return (sectionCount() == (int)m_filenames.size()) &&
    (regionLabelCount() == (int)m_region_colors.size()) &&
    (neuronTypeLabelCount() == (int)m_neuron_type_colors.size());
}

// Box

sl::aabox3f bsSectionedBrain::boundingBox() const {
  sl::aabox3f result;

  int n = sectionCount();
  if (n == 0) {
    result.to(sl::point3f(0.,0.,0.));
  } else {
    result.to_empty();
    for (int i=0; i<n; i++) {
      result.merge(sectionItem(i)->boundingBox());
    }
  }
  return result;
}

// Clear

void bsSectionedBrain::clear() {
  {
    for (int i=0; i<(int)m_brain_2d.size(); i++) {
      delete m_brain_2d[i];
      m_brain_2d[i]=NULL;
    }
    m_brain_2d.clear();
    m_filenames.clear();
  }
  {
    m_region_labels.clear();
    m_region_colors.clear();
    m_sub_region_volumes.clear();
  }
  {
    m_neuron_type_labels.clear();
    m_neuron_type_colors.clear();
    m_neuron_type_aligned.clear();
    m_neuron_type_enabled.clear();
    m_neuron_type_dimension.clear();
  }
}


// Queries -- Region Label list

int  bsSectionedBrain::maxSubRegionId() const {
  int result = -1;
  {
    for (int ns=0; ns<sectionCount(); ns++) {
      int id = sectionItem(ns)->maxSubRegionId();
      if (id > result) result=id;
    }
  }
  return result;
}

// Queries -- Region Label list

int  bsSectionedBrain::maxNeuronType() const {
  int result = -1;
  {
    for (int ns=0; ns<sectionCount(); ns++) {
      int id = sectionItem(ns)->maxNeuronType();
      if (id > result) result=id;
    }
  }
  return result;
}


// Command -- create color

sl::color3f bsSectionedBrain::createRegionColor(int i) {
  return bsGeometryHelpers::createColor(i);
}


sl::color3f bsSectionedBrain::createNeuronColor(int i) {
  sl::color3f c;
  switch (i%6) {
  case 0: c =   0.0f, 246.0f, 246.0f; break; // cyan
  case 1: c =   0.0f, 246.0f,   0.0f; break; // green
  case 2: c = 246.0f, 144.0f,   0.0f; break; // orange
  case 3: c = 246.0f,   0.0f,   0.0f; break; // red
  case 4: c = 246.0f,   0.0f, 246.0f; break; // magenta
  case 5: c =   0.0f,   0.0f, 246.0f; break; // blue
  }
  c[0] = c[0]/255.0f;
  c[1] = c[1]/255.0f;
  c[2] = c[2]/255.0f;
  return c;
}


// Command -- Region Label list

void  bsSectionedBrain::assignDefaultRegionLabels(int nmax) {
  assert(nmax>=0);
  {
    m_region_labels.clear();
    m_region_colors.clear();
  }
  while (!goodRegionLabelIndex(nmax)) {
    char ltmp[50];
    sprintf(ltmp,"Region-%03d",regionLabelCount());
    sl::color3f c = createRegionColor(regionLabelCount());
    m_region_labels.push_back(std::string(ltmp));
    m_region_colors.push_back(c);
  }
}

void  bsSectionedBrain::assignDefaultRegionLabels() {
  int nmax = maxSubRegionId();
  if (nmax >= 0) {
    assignDefaultRegionLabels(nmax);
  }
}


void  bsSectionedBrain::createRegionLabelColor(const std::string& label) {
  // char ltmp[50];
  // sprintf(ltmp,"Region-%03d",regionLabelCount());
  //  printf(ltmp,"Region-%03d",regionLabelCount());
   sl::color3f c = createRegionColor(regionLabelCount());
   m_region_colors.push_back(c);
  //  m_region_labels.push_back(std::string(ltmp));
   m_region_labels.push_back(std::string(label));
}

void  bsSectionedBrain::assignRegionLabel(int i, const std::string& label) {
  assert(i>=0  && i<regionLabelCount());
  m_region_labels[i]=label;
}


void  bsSectionedBrain::assignRegionColor(int i, const  sl::color3f& color) {
  SL_TRACE_OUT(0) << i << " " << regionLabelCount();
  assert(i>=0 && i<regionLabelCount());
  m_region_colors[i]=color;
}


void bsSectionedBrain::assignRegionLabelColor(int i, const std::string& label, const  sl::color3f& color) {
  assignRegionLabel(i, label);
  assignRegionColor(i, color);
}


int bsSectionedBrain::searchRegionLabel(const std::string& label) const{
  int result = -1;
  {
    for (int i=0; i<regionLabelCount() && result < 0; i++){
      if (regionLabelItem(i)==label) result = i;
    }
  }
  return result;
}


float bsSectionedBrain::brainVolume() const {
  float result = 0.;
  int n_sec = sectionCount();

  for (int i=0; i<n_sec; i++) {
    result += sectionItem(i)->sectionVolume();
  }
  return result;
}


float bsSectionedBrain::cortexRegionVolume(int c) const {
  float result = 0.;
  int n_sec = sectionCount();

  for (int i=0; i<n_sec; i++) {
    const bsSection *section = sectionItem(i);
    if (section->goodCortexRegionIndex(c)) {
      result += section->cortexRegionVolume(c);
    }
  }
  return result;
}


float bsSectionedBrain::subRegionVolume(int id) const {
  assert(id>=0 && id<(int)m_sub_region_volumes.size());
  return m_sub_region_volumes[id];
}


// Commands: sub-region volumes

void bsSectionedBrain::computeSubRegionVolumes() {

  m_sub_region_volumes.resize(regionLabelCount());
  std::fill(m_sub_region_volumes.begin(), m_sub_region_volumes.end(), 0.0f);

  int n_sec = sectionCount();

  for (int i=0; i<n_sec; i++) {
    const bsSection *section = sectionItem(i);
    const bsCortexRegion* cortex = section->cortexRegionItem(0);
    if (cortex) {
      for (int srn=0; srn < cortex->subRegionCount(); srn++) {
	const bsSubRegion* region = cortex->subRegionItem(srn);
	int id = region->getId();
	float v = region->regionVolume();
	appendVolume(id,v);
      }
    }
  }
}

void bsSectionedBrain::appendVolume(int id, float volume) {

  assert(id>=0);

  while ((int)m_sub_region_volumes.size() <= id) {
    m_sub_region_volumes.push_back(0.0f);
  }
  m_sub_region_volumes[id] += volume;
}


// Command -- Neuron Type Label list

void bsSectionedBrain::assignDefaultNeuronTypeLabels(int nmax) {
  assert(nmax>=0);
  {
    m_neuron_type_labels.clear();
    m_neuron_type_colors.clear();
    m_neuron_type_aligned.clear();
    m_neuron_type_enabled.clear();
    m_neuron_type_dimension.clear();
  }
  while (!goodNeuronTypeLabelIndex(nmax)) {
    char ltmp[50];
    sprintf(ltmp,"NeuronType-%03d",neuronTypeLabelCount());
    sl::color3f c = createNeuronColor(neuronTypeLabelCount());
    m_neuron_type_labels.push_back(strdup(ltmp));
    m_neuron_type_colors.push_back(c);
    m_neuron_type_aligned.push_back(false);
    m_neuron_type_enabled.push_back(true);
    m_neuron_type_dimension.push_back(0.0f);
  }
}

void bsSectionedBrain::assignDefaultNeuronTypeLabels() {
  int nmax = maxNeuronType();
  if (nmax >= 0) {
    assignDefaultNeuronTypeLabels(nmax);
  }
}

void bsSectionedBrain::createNeuronTypeLabel(const std::string& label, const bool b) {
    sl::color3f c = createNeuronColor(neuronTypeLabelCount());
    m_neuron_type_colors.push_back(c);
    m_neuron_type_labels.push_back(label);
    m_neuron_type_aligned.push_back(b);
    m_neuron_type_enabled.push_back(true);
    m_neuron_type_dimension.push_back(0.0f);
  // m_neuron_type_labels[i]=label;
  // m_neuron_type_aligned[i]=b;
  // setNeuronTypeDimension(i,0.0f);
}


void bsSectionedBrain::assignNeuronTypeLabel(int i, const std::string& label, const bool b) {
  assert(i>=0 && i<neuronTypeLabelCount());
  m_neuron_type_labels[i]=label;
  m_neuron_type_aligned[i]=b;
  setNeuronTypeDimension(i,0.0f);
}


void bsSectionedBrain::assignNeuronTypeColor(int i, const  sl::color3f& color, const bool b) {  
  assert(i>=0 && i<neuronTypeLabelCount());
  m_neuron_type_colors[i]=color;
  m_neuron_type_aligned[i]=b;
  setNeuronTypeDimension(i,0.0f);
}

void bsSectionedBrain::assignNeuronTypeLabelColor(int i, const std::string& label,
					     const  sl::color3f& color, 
					     const bool b) {
  assignNeuronTypeLabel(i, label, b);
  assignNeuronTypeColor(i, color, b);
}


void bsSectionedBrain::setNeuronTypeAligned(int i, bool b) {
  assert(goodNeuronTypeLabelIndex(i));
  m_neuron_type_aligned[i] = b;
}

void bsSectionedBrain::setNeuronTypeEnabled(int i, bool b) {
  assert(goodNeuronTypeLabelIndex(i));
  m_neuron_type_enabled[i] = b;
}

void bsSectionedBrain::setNeuronTypeDimension(int i, float value) {
  assert(goodNeuronTypeLabelIndex(i));
  while ((int)m_neuron_type_dimension.size() <= i) {
    m_neuron_type_dimension.push_back(0.0f);
  }
  m_neuron_type_dimension[i] = value;
}


std::vector<float> bsSectionedBrain::neuronTypeDimensions() const{
  return m_neuron_type_dimension;
}


bool bsSectionedBrain::isNeuronDimensionIndependent() const {
  bool result = false;
  for (int i=0; i<neuronTypeLabelCount() && !result; i++) {
    result = result || (neuronTypeDimension(i) != 0.0f);
  }
  return result;
}

int bsSectionedBrain::searchNeuronTypeLabel(const std::string& label) const{
  int result = -1;
  {
    for (int i=0; i<neuronTypeLabelCount() && result < 0; i++){
      if (neuronTypeLabelItem(i)==label) result = i;
    }
  }
  return result;
}


// Commands -- section
  
void bsSectionedBrain::appendSection(bsSection* s){
  assert(s);
  char fname[50];
  sprintf(fname,"F%03d.INS", sectionCount());
  appendSection(fname, s);
}

void bsSectionedBrain::appendSection(const std::string& fname, bsSection* s){
  assert(s);
  m_filenames.push_back(fname);
  m_brain_2d.push_back(s);
  assert(invariant());
}

void bsSectionedBrain::sort() {
  { 
    for (int n=sectionCount()-1; n>0; n--) {
      for (int i=0; i<n; i++) {
	if (m_brain_2d[i]->getQuota() > m_brain_2d[i+1]->getQuota()) {
	  bsSection *tmp=m_brain_2d[i];
	  m_brain_2d[i]=m_brain_2d[i+1];
	  m_brain_2d[i+1]=tmp;
	  std::string tmpstr=m_filenames[i];
	  m_filenames[i]=m_filenames[i+1];
	  m_filenames[i+1]=tmpstr;
	}
      }
    }
  }
  assignThickness();
}


void bsSectionedBrain::assignThickness() {
  int n=sectionCount();
  for (int i=0; i<n; i++) {
    const float EPS = 1e-5;
    float q=m_brain_2d[i]->getQuota();

    float qneuronup = q;
    {
      bool found = false;
      for (int j=i+1; j<n && ! found; j++) {
	if (! m_brain_2d[j]->ghostSection()) {
	  qneuronup= m_brain_2d[j]->getQuota();
	  found=true;
	}
      } 
    }

    float qneurondown = q;
    {
      bool found = false;
      for (int j=i-1; j>=0 && ! found; j--) {
	if (! m_brain_2d[j]->ghostSection()) {
	  qneurondown = m_brain_2d[j]->getQuota();
	  found=true;
	}
      } 
    }


    if (qneuronup-q < EPS)   qneuronup   = q + (q-qneurondown)*0.5f;
    if (q-qneurondown < EPS) qneurondown = q - (qneuronup-q)*0.5f;
    m_brain_2d[i]->setNeuronUpThickness((qneuronup-q)*0.5f);
    m_brain_2d[i]->setNeuronDownThickness((q-qneurondown)*0.5f);

    float qup= (i+1<n) ? m_brain_2d[i+1]->getQuota() : q;
    float qdown= (i-1>=0) ? m_brain_2d[i-1]->getQuota() : q;
    if (qup-q < EPS)   qup   = q + (q-qdown)*0.5f;
    if (q-qdown < EPS) qdown = q - (qup-q)*0.5f;
    m_brain_2d[i]->setUpThickness((qup-q)*0.5f);
    m_brain_2d[i]->setDownThickness((q-qdown)*0.5f);
  }
}

bool bsSectionedBrain::regularSection(int i) const {
  assert(goodSectionIndex(i));
  return sectionItem(i)->regularSection();
}

float bsSectionedBrain::sectionQuota(int i) const {
  assert(goodSectionIndex(i));
  return sectionItem(i)->getQuota();
}

