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
#include "bsDocument.h"
#include "bsSectionedBrain.h"
#include "bsBrainInsideIO.h"
#include "bsXmlIO.h"
#include "bsCmdLoadInside.h"
#include "bsCmdLoadXml.h"
#include "bsChooseOptions.h"
#include "qtCommandHistory.h"
#include "bsSection.h"

#include <sl/fixed_size_square_matrix.hpp>
#include <sl/rigid_body_map.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>

bsDocument::bsDocument(QObject* parent, qtCommandHistory* cmd_processor): QObject(parent) {
    m_cmd_processor=cmd_processor;
    m_brain2D=NULL;
    m_message="";
    m_current_filename="";
    m_error=0;
    m_bordersize = 0.0f;
    m_neuronsize = 0.0f;
    m_brain2D_modified=false;
    m_brain2D_saved=false;
    emit newBrain2D(false);
    connect(this, SIGNAL(regionOptionsChanged()), this, SLOT(computeSubRegionVolumes()));
    connect(this, SIGNAL(brain2DChanged(int)), this, SLOT(computeSubRegionVolumes()));
}
 
bsDocument::~bsDocument() {
}


bsSectionedBrain*  bsDocument::getBrain2D(){
    return m_brain2D;
}

void bsDocument::clear() {
    delete m_brain2D;
    m_brain2D=NULL;
    m_brain2D_modified=false;
}

void bsDocument::setErrorMessage(int error, const char* message){
    m_message=message;
    m_error=error;
    if(m_error != 0){
	emit errorOn();
    }
}

void bsDocument::transformSection(int i, const sl::affine_map2f& t){
    assert(m_brain2D->goodSectionIndex(i));
    bsSection* s = m_brain2D->sectionItem(i);
    s->transform(t);
    emit brain2DChanged(i);
    m_brain2D_modified=true;
}

void bsDocument::setRegionId(int section, int i, int j, int id) {
    assert(m_brain2D && m_brain2D->goodSectionIndex(section));
    bsSection* s = m_brain2D->sectionItem(section);
    assert(s->goodCortexRegionIndex(i));
    assert(j>=0 && j<s->subRegionCount(i));
    s->setRegionId(i,j,id);
    emit brain2DChanged(i);
    m_brain2D_modified=true;
}
 

void bsDocument::emitBrain2DChanged(int i){
  emit brain2DChanged(i);
  m_brain2D_modified=true;
}


void bsDocument::retriangulate(int i) {
  if (m_brain2D && m_brain2D->goodSectionIndex(i)) {
    bsSection* s = m_brain2D->sectionItem(i);
    s->triangulateSection();
    emit retriangulateSection(i);
  }
}


// Public Slots

void bsDocument::setBrain2D(bsSectionedBrain* b){
    if (m_brain2D) delete m_brain2D;
    m_brain2D = b;
    m_brain2D->computeSubRegionVolumes();
    emit newBrain2D(b!=NULL);
    emit regionOptionsChanged();
    emit neuronTypeOptionsChanged();
    m_brain2D_modified=false;
}

void bsDocument::loadInside(QString filename){
    bsCmdLoadInside* cmd = new bsCmdLoadInside(this, filename.toStdString().c_str());
    m_cmd_processor->doit(cmd);
}

void bsDocument::loadXml(QString filename){
  bsCmdLoadXml* cmd = new bsCmdLoadXml(this, filename.toStdString().c_str());
  m_cmd_processor->doit(cmd);
}

void bsDocument::saveInside(QString filename){
  bsBrainInsideIO inside;
  inside.save(filename.toStdString().c_str(),m_brain2D); 
  setErrorMessage(inside.lastErrorCode(), inside.lastMessage().c_str());
  m_brain2D_saved=(inside.lastErrorCode()==0);
}


void bsDocument::saveXml(QString filename){
  bsXmlIO xmlHandler;
  xmlHandler.save(filename.toStdString().c_str(),m_brain2D); 
  setErrorMessage(xmlHandler.lastErrorCode(), xmlHandler.lastMessage().c_str());
  m_brain2D_saved=(xmlHandler.lastErrorCode()==0);
}


void bsDocument::setRegionOptions(const bsChooseOptions* r) {
  if (m_brain2D) {
    for (int i=0; i < r->itemCount(); i++) {
      QString s = r->textItem(i);
      QColor qc = r->colorItem(i);
      sl::color3f c; c = qc.red()/255.0f, qc.green()/255.0f, qc.blue()/255.0f;
      m_brain2D->assignRegionLabelColor(i,s.toStdString(),c);
    }
    emit regionOptionsChanged(); 
  }
}

void bsDocument::setNeuronTypeOptions(const bsChooseOptions* r) {
  if (m_brain2D) {
    for (int i=0; i < r->itemCount(); i++) {
      QString s = r->textItem(i);
      QColor qc = r->colorItem(i);
      bool aligned = r->flagAligned(i);
      bool enabled = r->flagEnabled(i);
      float value = r->getDimension(i);
      sl::color3f c; c = qc.red()/255.0f, qc.green()/255.0f, qc.blue()/255.0f;
      m_brain2D->assignNeuronTypeLabelColor(i,s.toStdString(),c,aligned);
      m_brain2D->setNeuronTypeEnabled(i,enabled);
      m_brain2D->setNeuronTypeDimension(i,value);
    }
    emit neuronTypeOptionsChanged(); 
  }
}

void bsDocument::computeSubRegionVolumes() {
  if (m_brain2D) {
    m_brain2D->computeSubRegionVolumes();
  }
}

// Cutting Box and Settings
void bsDocument::setCuttingBox(sl::obox3f& cbox) {
  m_cutting_box = cbox;
}

void bsDocument::loadSettings(QString filename){
  std::ifstream input(filename.toStdString().c_str());
  if (!input.fail()) {
    sl::matrix4f m;
    input >> m;
    sl::vector3f v;
    input >> v;
    input.close();
    m_cutting_box=sl::obox3f(m,v);
    emit newCuttingBoxLoaded();
  } else {
  }
}  

void bsDocument::saveSettings(QString filename){
  std::ofstream output(filename.toStdString().c_str());
  if (!output.fail()) {
    sl::matrix4f m = m_cutting_box.from_box_space_map().as_matrix();
    for (int r=0; r < (int)m.row_count(); r++) {
      for (int c=0; c < (int)m.column_count(); c++) {
	output << m(r,c) << " ";
      }
      output << std::endl;
    }
    sl::vector3f v = m_cutting_box.half_side_lengths();
    output << v[0] << " " << v[1] << " " << v[2] << std::endl;
    output.close();
  } else {
  }
}

void bsDocument::setNeuronDimension(int i, float value) {
  if (hasBrain2D()) {
    m_brain2D->setNeuronTypeDimension(i,value);
  }
}

float bsDocument::getNeuronDimension(int i) const {
  assert(m_brain2D->goodNeuronTypeLabelIndex(i));
  return m_brain2D->neuronTypeDimension(i);
}

bool bsDocument::isNeuronDimensionIndependent() const {
  return m_brain2D->isNeuronDimensionIndependent();
}

void bsDocument::setGhostSection(int n, bool b) {
  if (m_brain2D->goodSectionIndex(n)) {
    bsSection* s = m_brain2D->sectionItem(n);
    s->setGhostSection(b);
    m_brain2D->assignThickness();
    // CHECK IF NECESSARY
    emit brain2DChanged(n);
  }
}


