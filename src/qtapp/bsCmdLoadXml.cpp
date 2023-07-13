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
#include "bsCmdLoadXml.h"
#include "bsDocument.h"
#include "bsXmlIO.h"
#include "bsSectionedBrain.h"

#include <string.h>
#include <stdio.h>


bsCmdLoadXml::bsCmdLoadXml(bsDocument* document, const char* filename){
  m_filename = strdup(filename);
  m_document = document;
}


bsCmdLoadXml::~bsCmdLoadXml(){
    if (m_filename) free(m_filename);
    m_filename=NULL;
}


const char* bsCmdLoadXml::name() const {
    return "Load Xml";
}


void bsCmdLoadXml::doit(){
  bsXmlIO xml_loader;
  bsSectionedBrain* new_brain2D = xml_loader.load(m_filename);
  int err = xml_loader.lastErrorCode();
  if (err == 0) {
      m_document->setBrain2D(new_brain2D);
      m_document->setCurrentFilename(m_filename);
  }
  else {
      if (new_brain2D) delete new_brain2D;
  }
  m_document->setErrorMessage(xml_loader.lastErrorCode(), xml_loader.lastMessage().c_str());
}

