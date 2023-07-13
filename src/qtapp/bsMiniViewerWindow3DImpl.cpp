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
#include "bsMiniViewerWindow3DImpl.h"
#include "ui_bsMiniViewerWindow3D.hpp"
#include "bsFastBrainViewer.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QBoxLayout>
#include <QHBoxLayout>
#include "bsDocument.h"

/* 
 *  Constructs a bsMiniViewerWindow3DImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
bsMiniViewerWindow3DImpl::bsMiniViewerWindow3DImpl( QWidget* parent,  qtCommandHistory* cmd_processor )
  : QWidget( parent ), ui(new Ui::bsMiniViewerWindow3D)
{
  ui->setupUi(this);
  m_cmd_processor = (cmd_processor) ? (cmd_processor) : new qtCommandHistory(-1);
  ui->m_viewer->setCommandProcessor(cmd_processor);
  m_document=NULL;
  m_tool_layout = new QHBoxLayout(ui->m_tool_frame);
  m_tool_layout->setSpacing(6);
  m_tool_layout->setMargin(5);
  enableAllWidgets(false);

}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsMiniViewerWindow3DImpl::~bsMiniViewerWindow3DImpl()
{
    // no need to delete child widgets, Qt does it all for us
  delete ui;
}


bsViewer* bsMiniViewerWindow3DImpl::getViewer(){
  return ui->m_viewer;
}

bsFastBrainViewer* bsMiniViewerWindow3DImpl::getViewer3D(){
  return ui->m_viewer;
}

QBoxLayout* bsMiniViewerWindow3DImpl::getToolLayout() {
  return m_tool_layout;
}

QFrame* bsMiniViewerWindow3DImpl::getToolFrame() {
  return ui->m_tool_frame;
}

bsDocument* bsMiniViewerWindow3DImpl::getDocument(){
    return m_document;
}

void bsMiniViewerWindow3DImpl::setDocument(bsDocument* doc){
    m_document=doc;
    ui->m_viewer->setDocument(doc);
    connect(m_document,SIGNAL(newBrain2D(bool)),this,SLOT(enableAllWidgets(bool)));
}

void bsMiniViewerWindow3DImpl::setEnableSubRegions(bool b) {
  ui->m_viewer->setEnableSubRegions(b);
}

void bsMiniViewerWindow3DImpl::setEnableNeuronTypes(bool b) {
  ui->m_viewer->setEnableNeuronTypes(b);
}

void bsMiniViewerWindow3DImpl::setEnableSelectedRenderer(bool b) {
  ui->m_viewer->setEnableSelectedRenderer(b);
}

void  bsMiniViewerWindow3DImpl::setSelectedRendererIndex(int n) {
  ui->m_viewer->setSelectedRendererIndex(n);
}

void  bsMiniViewerWindow3DImpl::enableAllWidgets(bool b){
  //  m_draft_quality_button->setEnabled(b);
  ui->m_reset_button->setEnabled(b);
}

