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
#include "bsIsoViewerWindow3DImpl.h"
#include "bsIsoBrainViewer.h"
#include "qtCommandHistory.h"
#include "bsDocument.h"

#include <qobject.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qradiobutton.h>
#include <qslider.h>

/* 
 *  Constructs a bsIsoViewerWindow3DImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
bsIsoViewerWindow3DImpl::bsIsoViewerWindow3DImpl( QWidget* parent,  const char* name,
					    WFlags fl, qtCommandHistory* cmd_processor )
    : bsIsoViewerWindow3D( parent, name, fl )
{    
  m_cmd_processor = (cmd_processor) ? (cmd_processor) : new qtCommandHistory(-1);
  m_viewer->setCommandProcessor(cmd_processor);
  m_document=NULL;
  m_tool_layout = new QHBoxLayout(m_tool_frame);
  m_tool_layout->setSpacing(6);
  m_tool_layout->setMargin(5);
  enableAllWidgets(false);

}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsIsoViewerWindow3DImpl::~bsIsoViewerWindow3DImpl()
{
    // no need to delete child widgets, Qt does it all for us
}


bsViewer* bsIsoViewerWindow3DImpl::getViewer(){
  return m_viewer;
}

bsIsoBrainViewer* bsIsoViewerWindow3DImpl::getViewer3D(){
  return m_viewer;
}

QBoxLayout* bsIsoViewerWindow3DImpl::getToolLayout() {
  return m_tool_layout;
}

QFrame* bsIsoViewerWindow3DImpl::getToolFrame() {
  return m_tool_frame;
}

bsDocument* bsIsoViewerWindow3DImpl::getDocument(){
    return m_document;
}

void bsIsoViewerWindow3DImpl::setDocument(bsDocument* doc){
    m_document=doc;
    m_viewer->setDocument(doc);
    connect(m_document,SIGNAL(newBrain2D(bool)),this,SLOT(enableAllWidgets(bool)));
}

void bsIsoViewerWindow3DImpl::setEnableSubRegions(bool b) {
  m_viewer->setEnableSubRegions(b);
}

void bsIsoViewerWindow3DImpl::setEnableNeuronTypes(bool b) {
  m_viewer->setEnableNeuronTypes(b);
}

void  bsIsoViewerWindow3DImpl::enableAllWidgets(bool b){
  m_reset_button->setEnabled(b);
  m_rebuild_button->setEnabled(b);
  m_enable_subregions->setEnabled(b);
  m_enable_neurons->setEnabled(b);
  m_enable_cutting_planes->setEnabled(b);
  m_extinction_zeta->setEnabled(b);
}

