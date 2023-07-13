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
#include "bsHQIsoViewerWindow3DImpl.h"
#include "ui_bsHQIsoViewerWindow3D.hpp"
#include "bsHQIsoBrainViewer.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QFrame>
#include <QBoxLayout>
#include <QHBoxLayout>
#include "bsDocument.h"

/* 
 *  Constructs a bsHQIsoViewerWindow3DImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
bsHQIsoViewerWindow3DImpl::bsHQIsoViewerWindow3DImpl( QWidget* parent, qtCommandHistory* cmd_processor )
  : QWidget( parent ), ui(new Ui::bsHQIsoViewerWindow3D) {
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
bsHQIsoViewerWindow3DImpl::~bsHQIsoViewerWindow3DImpl()
{
    // no need to delete child widgets, Qt does it all for us
  delete ui;
}

bsViewer* bsHQIsoViewerWindow3DImpl::getViewer(){
  return ui->m_viewer;
}

bsHQIsoBrainViewer* bsHQIsoViewerWindow3DImpl::getViewer3D(){
  return ui->m_viewer;
}

QBoxLayout* bsHQIsoViewerWindow3DImpl::getToolLayout() {
  return m_tool_layout;
}

QFrame* bsHQIsoViewerWindow3DImpl::getToolFrame() {
  return ui->m_tool_frame;
}

bsDocument* bsHQIsoViewerWindow3DImpl::getDocument(){
    return m_document;
}

void bsHQIsoViewerWindow3DImpl::setDocument(bsDocument* doc){
    m_document=doc;
    ui->m_viewer->setDocument(doc);
    connect(m_document,SIGNAL(newBrain2D(bool)),this,SLOT(enableAllWidgets(bool)));
}

void bsHQIsoViewerWindow3DImpl::setEnableSubRegions(bool b) {
  ui->m_viewer->setEnableSubRegions(b);
}

void bsHQIsoViewerWindow3DImpl::setEnableNeuronTypes(bool b) {
  ui->m_viewer->setEnableNeuronTypes(b);
}

void  bsHQIsoViewerWindow3DImpl::enableAllWidgets(bool b){
  ui->m_reset_button->setEnabled(b);
  ui->m_reset_rear->setEnabled(b);
  ui->m_reset_right->setEnabled(b);
  ui->m_reset_left->setEnabled(b);
  ui->m_reset_top->setEnabled(b);
  ui->m_reset_bottom->setEnabled(b);
  ui->m_rebuild_button->setEnabled(b);
  ui->m_enable_subregions->setEnabled(b);
  ui->m_enable_neurons->setEnabled(b);
  ui->m_enable_cutting_planes->setEnabled(b);
  ui->m_extinction_zeta->setEnabled(b);
  ui->m_extinction_zeta_threshold_start->setEnabled(b);
  ui->m_extinction_zeta_threshold_end->setEnabled(b);
  ui->m_enable_cutting_caps->setEnabled(b);
  ui->m_enable_light_two_side->setEnabled(b);
}
