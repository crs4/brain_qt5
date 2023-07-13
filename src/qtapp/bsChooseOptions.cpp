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
#include "bsChooseOptions.h"
#include "bsChooseOptionsItem.h"
#include "bsGeometryHelpers.h"

#include <cassert>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>

// WhatIs text 

const char * sizeText =
"This value changes the thickness of the geometric elements in the 2D viewer.\n"
"0 (zero) means values are calculated automatically";


bsChooseOptions::bsChooseOptions( QWidget* parent, bool show_flag)
    : QDialog( parent )
{
    m_show_flag=show_flag;

    resize( 412, 480 ); 
    setWindowTitle( tr( "Choose Options" ) );

    bsChooseOptionsLayout = new QVBoxLayout( this ); 
    bsChooseOptionsLayout->setSpacing( 6 );
    bsChooseOptionsLayout->setMargin( 11 );

    m_color_group_box = new QGroupBox(this);
    m_color_group_box_layout = new QVBoxLayout(m_color_group_box);
    bsChooseOptionsLayout->addWidget( m_color_group_box );

    Layout4 = new QHBoxLayout; 
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );

    m_dimension_frame = new QFrame( this );
    //m_dimension_frame->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, m_dimension_frame->sizePolicy().hasHeightForWidth() ) );
    m_dimension_frame->setMinimumSize( QSize( 0, 45 ) );
    m_dimension_frame->setFrameShape( QFrame::StyledPanel );
    m_dimension_frame->setFrameShadow( QFrame::Raised );
    m_dimension_frameLayout = new QHBoxLayout( m_dimension_frame ); 
    m_dimension_frameLayout->setSpacing( 6 );
    m_dimension_frameLayout->setMargin( 11 );

    m_dimension_label = new QLabel( m_dimension_frame );
    m_dimension_label->setText( tr( "Thickness:" ) );
    m_dimension_frameLayout->addWidget( m_dimension_label );

    m_dimension = new QSpinBox( m_dimension_frame );
    m_dimension->setMaximum( 10 );
    m_dimension->setToolTip( "Set thickness. 0=AUTO" );
    m_dimension->setWhatsThis( sizeText );
    //QToolTip::add(  m_dimension, tr( "Set thickness. 0=AUTO" ) );
    //Q3WhatsThis::add( m_dimension, sizeText );

    m_dimension_frameLayout->addWidget( m_dimension );

    Layout4->addWidget( m_dimension_frame );

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer );

    bsChooseOptionsLayout->addLayout( Layout4 );

    Line1 = new QFrame( this );
    Line1->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    bsChooseOptionsLayout->addWidget( Line1 );

    m_buttons_layout = new QHBoxLayout; 
    m_buttons_layout->setSpacing( 6 );
    m_buttons_layout->setMargin( 0 );

    m_close_button = new QPushButton( this );
    m_close_button->setText( tr( "OK" ) );
    m_buttons_layout->addWidget( m_close_button );

    m_cancel_button = new QPushButton( this );
    m_cancel_button->setText( tr( "Cancel" ) );
    m_buttons_layout->addWidget( m_cancel_button );

    m_add_button = new QPushButton( this );
    m_add_button->setText( tr( "Add Item" ) );
    m_buttons_layout->addWidget( m_add_button );
    //m_buttons_layout->addWidget(QWhatsThis::whatsThisButton( this ));

    bsChooseOptionsLayout->addLayout( m_buttons_layout );
    
    // signals and slots connections
    connect( m_close_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( m_add_button, SIGNAL( clicked() ), this, SLOT( addItem() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsChooseOptions::~bsChooseOptions()
{
    // no need to delete child widgets, Qt does it all for us
  clear();
}


void bsChooseOptions::clear() {
  for (int i=0; i< itemCount(); i++) {
    if(m_list[i]) delete m_list[i];
    m_list[i] = NULL;
  }
  m_list.clear();
}


void bsChooseOptions::addItem() {
  sl::color3f c = bsGeometryHelpers::createColor((int)m_list.size());
  addItem(QColor((int)(c[0]*255.0f),(int)(c[1]*255.0f),(int)(c[2]*255.0f)),"NEW ITEM",false);
}


void bsChooseOptions::addItem(const QColor c, const QString s, const bool aligned, const bool enabled, const float dimension) {
  bsChooseOptionsItem* f = new bsChooseOptionsItem( m_color_group_box, m_show_flag);
  //bsChooseOptionsItem* f = new bsChooseOptionsItem( m_color_group, "f", 0, m_show_flag);
  //f->setGeometry( QRect( 11, 19+m_list.size()*50, 330, 45 ) ); 
  f->setColor(c);
  f->setText(s);
  f->setFlagAligned(aligned);
  f->setFlagEnabled(enabled);
  f->setSize((int)dimension);
  f->show();
  m_list.push_back(f);
  m_color_group_box_layout->addWidget(f);
}

QColor bsChooseOptions::colorItem(const int i) const {
  assert(goodItemIndex(i));
  return m_list[i]->getColor();
}

QString bsChooseOptions::textItem(const int i) const {
  assert(goodItemIndex(i));
  return m_list[i]->getText();
}
 
bool bsChooseOptions::flagAligned(const int i) const {
  assert(goodItemIndex(i));
  return m_list[i]->getFlagAligned();
}

bool bsChooseOptions::flagEnabled(const int i) const {
  assert(goodItemIndex(i));
  return m_list[i]->getFlagEnabled();
}

float bsChooseOptions::getDimension(const int i) const {
  assert(goodItemIndex(i));
  return (float)m_list[i]->getSize();
}

float bsChooseOptions::getDimension() const {
  return (float)m_dimension->value();
}



