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
#include "bsCmdTransformSection.h"
#include "bsTransformationTool.h"
#include <sl/projective_map.hpp>

bsCmdTransformSection::bsCmdTransformSection(bsTransformationTool* t,
					     const char transform_type,
					     const sl::affine_map2f& m) {
    m_tool = t;
    m_type = transform_type;
    m_transform = m;
}

bsCmdTransformSection::~bsCmdTransformSection(){
}

const char* bsCmdTransformSection::name() const {
  switch (m_type) {
  case 'r': return "Rotate"; break;
  case 't': return "Translate"; break;
  }
  return NULL;
}  

void bsCmdTransformSection::redo() {
    m_tool->transformSectionCmd(m_transform);
}

void bsCmdTransformSection::undo() {
    m_tool->transformSectionCmd(m_transform.inverse()); 
}

void bsCmdTransformSection::backup() {
}

bool bsCmdTransformSection::isComposable( slCommand *c ) const {
  assert(c);
  
  bool result = false;
  bsCmdTransformSection* c_new = dynamic_cast<bsCmdTransformSection*>(c);
  if (c_new) {
    result = m_type == c_new->m_type; 
  }

  return result;
}

void bsCmdTransformSection::composeAndDestroy( slCommand *c) {
  assert(c);
  assert(!c->isLinked());
  assert(isComposable(c));

  bsCmdTransformSection* c_new = dynamic_cast<bsCmdTransformSection*>(c);
  
  m_transform = m_transform * c_new->m_transform;

  delete c;
}

