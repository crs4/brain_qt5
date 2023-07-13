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
#include "bsViewerWindow2DImpl.h"
#include "ui_bsViewerWindow2D.hpp"
#include "bsSectionedBrain.h"
#include "bsViewer2DOptionsImpl.h"
#include "bsSection.h"

#include "bsSectionViewer2D.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include "bsDocument.h"

#include <qobject.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qslider.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

/* 
 *  Constructs a bsViewerWindow2DImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
bsViewerWindow2DImpl::bsViewerWindow2DImpl( QWidget* parent, qtCommandHistory* cmd_processor ) : QWidget (parent), ui(new Ui::bsViewerWindow2D)
{
  ui->setupUi(this);
  m_cmd_processor = (cmd_processor) ? (cmd_processor) : new qtCommandHistory(-1);
  ui->m_viewer->setCommandProcessor(cmd_processor);
  m_document=NULL;
    ui->upSection->setEnabled(false);
    ui->downSection->setEnabled(false);
    ui->m_zoom_slider->setEnabled(false);
    ui->m_reset_button->setEnabled(false);
    ui->m_x_slider->setEnabled(false);
    ui->currentSection->setEnabled(false);
    ui->m_y_slider->setEnabled(false);
    ui->m_fill_button->setEnabled(false);
    ui->m_cut_button->setEnabled(false);
    ui->m_quota_label->setEnabled(false);
    ui->m_grid->setEnabled(false);
    ui->m_ghost_button->setEnabled(false);
    //m_image_button->setEnabled(false);

    m_tool_layout = new QHBoxLayout( ui->m_tool_frame ); 
    m_tool_layout->setSpacing( 6 );
    m_tool_layout->setMargin( 5 );

    m_option_dialog = new bsViewer2DOptionsImpl(this);
      
    connect( ui->m_viewer, SIGNAL( currentSectionChanged(int) ), this, SIGNAL( currentSectionChanged(int) ));
    connect( ui->m_viewer, SIGNAL( currentSectionChanged(int) ), this, SLOT(updateIntersectionInfo(int) ));
    connect( ui->m_viewer, SIGNAL( currentSectionChanged(int) ), this, SLOT(updateQuotaInfo(int) ));
    connect( ui->m_viewer, SIGNAL( currentSectionChanged(int) ), this, SLOT(updateGhostInfo(int) ));

    connect( ui->m_option_button, SIGNAL( clicked() ), this, SLOT(showOptionDialog()));

}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsViewerWindow2DImpl::~bsViewerWindow2DImpl()
{
  // no need to delete child widgets, Qt does it all for us
  delete ui;
}

bsDocument* bsViewerWindow2DImpl::getDocument(){
    return m_document;
}

void bsViewerWindow2DImpl::setDocument(bsDocument* doc){
    m_document=doc;
    ui->m_viewer->setDocument(doc);
    connect(m_document,SIGNAL(newBrain2D(bool)),this,SLOT(setNewBrain()));
    connect(m_document,SIGNAL(retriangulateSection(int)),this,SLOT(updateIntersectionInfo(int)));
    connect(ui->m_ghost_button,SIGNAL(toggled(bool)),this,SLOT(setGhostSection(bool)));
}

void bsViewerWindow2DImpl::setNewBrain() {
    bsSectionedBrain* b=m_document->getBrain2D();
    if (b) {
	ui->currentSection->setMaximum(b->sectionCount()-1);
	ui->currentSection->setValue(0);
	ui->upSection->setValue(0);
	ui->downSection->setValue(0);
	updateIntersectionInfo(0);
	updateQuotaInfo(0);
    }
    ui->m_zoom_slider->setEnabled(b);
    ui->m_reset_button->setEnabled(b);
    ui->m_x_slider->setEnabled(b);
    ui->currentSection->setEnabled(b);
    ui->m_y_slider->setEnabled(b);
    ui->upSection->setEnabled(b);
    ui->downSection->setEnabled(b);
    ui->m_fill_button->setEnabled(b);
    ui->m_cut_button->setEnabled(b);
    ui->m_quota_label->setEnabled(b);
    ui->m_grid->setEnabled(b);
    ui->m_ghost_button->setEnabled(b);
    //m_image_button->setEnabled(b);
}


bsViewer* bsViewerWindow2DImpl::getViewer(){
    return ui->m_viewer;
}

bsSectionViewer2D* bsViewerWindow2DImpl::getViewer2D(){
    return ui->m_viewer;
}


int  bsViewerWindow2DImpl::getCurrentSectionIndex(){
  return ui->m_viewer->currentSection();
}

QBoxLayout* bsViewerWindow2DImpl::getToolLayout() {
  return m_tool_layout;
}

QFrame* bsViewerWindow2DImpl::getToolFrame() {
  return ui->m_tool_frame;
}

void bsViewerWindow2DImpl::updateIntersectionInfo(int n_section) {
  bsSectionedBrain* b=m_document->getBrain2D();
  if(b && b->goodSectionIndex(n_section) && !b->regularSection(n_section)) {
    ui->m_error_info->setStyleSheet("QLabel { background-color : red; }");
    ui->m_error_info->setText("NOT OK");
  } else {
    ui->m_error_info->setStyleSheet("QLabel { background-color : #F3F3F3; }");
    ui->m_error_info->setText("");
  }
}


void bsViewerWindow2DImpl::updateQuotaInfo(int n_section) {
  bsSectionedBrain* b=m_document->getBrain2D();
    QString str;
  if(b && b->goodSectionIndex(n_section)) {
    str=QString::asprintf("%7.0f",b->sectionQuota(n_section));
  } else {
    str="";
  }
  ui->m_quota_label->setText(str);
}

void bsViewerWindow2DImpl::updateGhostInfo(int n_section) {
  bsSectionedBrain* b=m_document->getBrain2D();
  QString str;
  if(b && b->goodSectionIndex(n_section)) {
    bsSection* s = b->sectionItem(n_section);
    ui->m_ghost_button->setChecked(s->ghostSection());
  }
}


void bsViewerWindow2DImpl::showOptionDialog() {
  m_option_dialog->exec();
  ui->m_viewer->setCellStepX(m_option_dialog->getCellStepX());
  ui->m_viewer->setCellStepY(m_option_dialog->getCellStepY());
}


void bsViewerWindow2DImpl::setGhostSection(bool b) {
  m_document->setGhostSection(ui->m_viewer->currentSection(),b);
}
