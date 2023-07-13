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
#include "bsViewer2DOptionsImpl.h"
#include "ui_bsViewer2DOptions.hpp"

#include "qstring.h"
#include <qlineedit.h>


/* 
 *  Constructs a bsViewer2DOptionsImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
bsViewer2DOptionsImpl::bsViewer2DOptionsImpl( QWidget* parent )
  : QDialog(parent), ui(new Ui::bsViewer2DOptions) {
  ui->setupUi(this);
}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsViewer2DOptionsImpl::~bsViewer2DOptionsImpl()
{
    // no need to delete child widgets, Qt does it all for us
  delete ui;
}

float bsViewer2DOptionsImpl::getCellStepX() const {
  bool ok;
  float val;
  val = ui->m_cell_stepX->text().toFloat(&ok);
  if (!ok) {
    val = 10000.0f;
  }
  QString str;
  str=QString::asprintf("%.0f", val);
  ui->m_cell_stepX->setText(str);
  return val;
}


float bsViewer2DOptionsImpl::getCellStepY() const {
  bool ok;
  float val;
  val = ui->m_cell_stepY->text().toFloat(&ok);
  if (!ok) {
    val = 10000.0f;
  }
  QString str;
  str=QString::asprintf("%.0f", val);
  ui->m_cell_stepY->setText(str);
  return val;
}

void bsViewer2DOptionsImpl::show() {
  show();
  m_old_cell_stepX=getCellStepX();
  m_old_cell_stepY=getCellStepY();
}

void bsViewer2DOptionsImpl::reject(){
  reject();
  QString str;
  str=QString::asprintf("%.0f", m_old_cell_stepX);
  ui->m_cell_stepX->setText(str);
  str=QString::asprintf("%.0f", m_old_cell_stepY);
  ui->m_cell_stepY->setText(str);
}
