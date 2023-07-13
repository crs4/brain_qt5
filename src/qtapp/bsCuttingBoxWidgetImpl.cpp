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
#include "bsCuttingBoxWidgetImpl.h"
#include "ui_bsCuttingBoxWidget.hpp"

#include <iostream>
#include <qstring.h>
#include <qslider.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

/* 
 *  Constructs a bsCuttingBoxWidgetImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
bsCuttingBoxWidgetImpl::bsCuttingBoxWidgetImpl( QWidget* parent )
  : QWidget( parent), ui(new Ui::bsCuttingBoxWidget) {
  ui->setupUi(this);
  m_width = 0.0f;
  m_length = 0.0f;
  m_height = 0.0f;

  connect(ui->m_w, SIGNAL(returnPressed()), this, SLOT(emit_wlhChanged()));
  connect(ui->m_l, SIGNAL(returnPressed()), this, SLOT(emit_wlhChanged()));
  connect(ui->m_h, SIGNAL(returnPressed()), this, SLOT(emit_wlhChanged()));

  connect(ui->m_slider_i,SIGNAL(valueChanged(int)), this, SLOT(emit_slidersChanged(int)));
  connect(ui->m_slider_j,SIGNAL(valueChanged(int)), this, SLOT(emit_slidersChanged(int)));
  connect(ui->m_slider_k,SIGNAL(valueChanged(int)), this, SLOT(emit_slidersChanged(int)));

  connect(ui->m_reset_button, SIGNAL(clicked()), this, SLOT(emit_resetClicked()));
  connect(ui->m_reset_button, SIGNAL(clicked()), this, SLOT(resetSliders()));

}


/*  
 *  Destroys the object and frees any allocated resources
 */
bsCuttingBoxWidgetImpl::~bsCuttingBoxWidgetImpl()
{
    // no need to delete child widgets, Qt does it all for us
  delete ui;
}

void bsCuttingBoxWidgetImpl::setW(const float val) {
  QString str;
  str=QString::asprintf("%.0f", val);
  ui->m_w->setText(str);
  m_width=val;
}

void bsCuttingBoxWidgetImpl::setL(const float val) {
  QString str;
  str=QString::asprintf("%.0f", val);
  ui->m_l->setText(str);
  m_length=val;
}

void bsCuttingBoxWidgetImpl::setH(const float val) {
  QString str;
  str=QString::asprintf("%.0f", val);
  ui->m_h->setText(str);
  m_height=val;
}


void bsCuttingBoxWidgetImpl::emit_wlhChanged() {
  bool ok;
  float val;
  val = ui->m_w->text().toFloat(&ok);
  setW(ok ? val : m_width);

  val = ui->m_l->text().toFloat(&ok);
  setL(ok ? val : m_length);

  val = ui->m_h->text().toFloat(&ok);
  setH(ok ? val : m_height);
  
  emit wlhChanged();
}


void bsCuttingBoxWidgetImpl::emit_resetClicked() {
  emit resetClicked();
}

void bsCuttingBoxWidgetImpl::emit_slidersChanged(int v) {
  if(v!=0) {
    emit slidersChanged();
  }
}

void bsCuttingBoxWidgetImpl::resetSliders() {
  ui->m_slider_i->setValue(0);
  ui->m_slider_j->setValue(0);
  ui->m_slider_k->setValue(0);
  enableSliders(true);
}

void bsCuttingBoxWidgetImpl::enableSliders(bool b) {
  ui->m_slider_i->setEnabled(b);
  ui->m_slider_j->setEnabled(b);
  ui->m_slider_k->setEnabled(b);
  if(b) {
    ui->m_slider_i->show();
    ui->m_slider_j->show();
    ui->m_slider_k->show();
    ui->m_label_i->show();
    ui->m_label_j->show();
    ui->m_label_k->show();
  } else {
    ui->m_slider_i->hide();
    ui->m_slider_j->hide();
    ui->m_slider_k->hide();
    ui->m_label_i->hide();
    ui->m_label_j->hide();
    ui->m_label_k->hide();
  }
}


float bsCuttingBoxWidgetImpl::normalizeSlider(int i) const {
  QSlider* tmp=NULL;
  switch (i) {
  case 0: tmp=ui->m_slider_i;
    break;
  case 1: tmp=ui->m_slider_j;
    break;
  case 2: tmp=ui->m_slider_k;
    break;
  }
  float maxV=tmp->maximum();
  float v = tmp->value();
  return (v/maxV);
}


void bsCuttingBoxWidgetImpl::setSliders(float vi, float vj, float vk) {
  ui->m_slider_i->setValue(int(vi*ui->m_slider_i->maximum()));
  ui->m_slider_j->setValue(int(vj*ui->m_slider_j->maximum()));
  ui->m_slider_k->setValue(int(vk*ui->m_slider_k->maximum()));
}
