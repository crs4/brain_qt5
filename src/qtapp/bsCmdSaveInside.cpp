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
#include "bsCmdSaveInside.h"
#include "bsDocument.h"
#include "bsBrainInsideIO.h"
#include "bsSectionedBrain.h"
#include <string.h>
#include <stdio.h>


bsCmdSaveInside::bsCmdSaveInside(bsDocument* document, const char* filename){
  m_filename = strdup(filename);
  m_document = document;
}


bsCmdSaveInside::~bsCmdSaveInside(){
    if (m_filename) free(m_filename);
    m_filename=NULL;
}


const char* bsCmdSaveInside::name() const {
    return "Save";
}


void bsCmdSaveInside::doit(){
  bsBrainInsideIO inside;
  inside.save(m_filename,m_brain2D); 
  m_document->setErrorMessage(inside.lastErrorCode(), inside.lastMessage());
}

