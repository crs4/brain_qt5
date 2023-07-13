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
#include "bsAssignWidget.h"

#include <assert.h>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>

#include <iostream>

bsAssignWidget::bsAssignWidget( QWidget* parent)
    : QDialog( parent )
{
    resize( 330, 244 ); 
    setWindowTitle( tr( "Assign SubRegion Id"  ) );
    setSizeGripEnabled(true);
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    vbox = new QVBoxLayout; 
    vbox->setSpacing( 6 );
    vbox->setMargin( 0 );

    m_ok_button = new QPushButton( tr( "&OK"  ), this );
    m_ok_button->setAutoDefault( true );
    m_ok_button->setDefault( true );
    vbox->addWidget( m_ok_button );

    m_cancel_button = new QPushButton( tr( "&Cancel"  ), this );
    m_cancel_button->setText( tr( "&Cancel"  ) );
    m_cancel_button->setAutoDefault( true );
    vbox->addWidget( m_cancel_button );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    vbox->addItem( spacer );

    grid->addLayout( vbox, 0, 1 );

    m_list_box = new QListWidget( this );

    grid->addWidget( m_list_box, 0, 0 );

    // signals and slots connections
    connect( m_ok_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsAssignWidget::~bsAssignWidget()
{
    // no need to delete child widgets, Qt does it all for us
}


void bsAssignWidget::setItem(const char* label){
    assert(label);
    m_list_box->addItem( tr(label) );
}

void bsAssignWidget::setCurrentItem(int id) {
  m_list_box->setCurrentItem(m_list_box->item(id));
}

void bsAssignWidget::accept(){
    QDialog::accept();
    setResult(m_list_box->currentRow());
}

void bsAssignWidget::reject(){
    QDialog::reject();
    setResult(-1);
}


