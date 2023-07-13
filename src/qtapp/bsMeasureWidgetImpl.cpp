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
#include "bsMeasureWidgetImpl.h"
#include "ui_bsMeasureWidget.hpp"
#include <qstring.h>
#include <qlineedit.h>
#include <iostream>

/* 
 *  Constructs a bsMeasureWidgetImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
bsMeasureWidgetImpl::bsMeasureWidgetImpl( QWidget* parent )
  : QWidget( parent ), ui(new Ui::bsMeasureWidget)
{
  ui->setupUi(this);
  resize( 250, 200 );
  ui->m_start_point->setReadOnly(true);
  ui->m_end_point->setReadOnly(true);
  ui->m_distance->setReadOnly(true);
}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsMeasureWidgetImpl::~bsMeasureWidgetImpl()
{
    // no need to delete child widgets, Qt does it all for us
  delete ui;
}

void bsMeasureWidgetImpl::setStartPoint(QString& str){
    ui->m_start_point->setText(str);
}
void bsMeasureWidgetImpl::setEndPoint(QString& str){
    ui->m_end_point->setText(str);
}
void bsMeasureWidgetImpl::setDistance(QString& str){
    ui->m_distance->setText(str);
}

