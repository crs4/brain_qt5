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
#include "bsEditor.h"
#include "bsViewer.h"
#include "qtCommandHistory.h"
#include "bsDocument.h"
#include "bsTool.h"

#include <assert.h>

#include <QFrame>
#include <QBoxLayout>

#include <sl/assert.hpp>
#include <iostream>

bsEditor::bsEditor( QWidget* parent, qtCommandHistory* cmd_processor)
    : QWidget( parent ) {

  m_cmd_processor=cmd_processor;
  m_current_tool = NULL;
  m_document=NULL;
}

bsEditor::~bsEditor() {
  SL_TRACE_OUT(0) << std::endl;
  for(std::size_t i=0; i<<m_tools.size(); ++i) {
    if(m_tools[i]) delete m_tools[i];
    m_tools[i]=0;
  }
  m_tools.clear();
}

bsTool* bsEditor::getCurrentTool() {
    return m_current_tool;
}

void bsEditor::setCurrentTool(bsTool* tool) {
    if (tool != m_current_tool) {
	bsTool* old_tool = m_current_tool;
	m_current_tool = tool;
	if (old_tool) {
	    old_tool->reset();
	}	    
    }
}

void bsEditor::insertTool(bsTool* tool) {
   assert(tool);
   getToolLayout()->addWidget( tool->getToolButton() );
   m_tools.push_back(tool);
}

    
void bsEditor::resetAllTools(){
    bool b = m_document && m_document->hasBrain2D();
    for (int i=0; i<(int)m_tools.size(); i++) {
	m_tools[i]->reset();
	m_tools[i]->enableTool(b);
    }
}

void bsEditor::enableAllTools(bool b){
    for (int i=0; i<(int)m_tools.size(); i++) {
	m_tools[i]->enableTool(b);
    }
}

void  bsEditor::setViewCursor(QCursor c){
    getViewer()->setCursor(c);
}


void  bsEditor::setDocument(bsDocument* doc){
  if (m_document) delete m_document;
  m_document=doc;
  setDocumentViewerWindow();
}


bsDocument* bsEditor::getDocument() {
  return m_document;
}


qtCommandHistory* bsEditor::getCmdProcessor() {
  return m_cmd_processor;
}

void bsEditor::updateViewer(){
  getViewer()->updateGL();
}
