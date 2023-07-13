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
/****************************************************************************
** Form implementation generated from reading ui file 'test.ui'
**
** Created: Sat Jun 16 12:13:56 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "test.h"

#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a m_color_chooser which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
m_color_chooser::m_color_chooser( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "m_color_chooser" );
    resize( 412, 480 ); 
    setCaption( tr( "Options" ) );
    m_color_chooserLayout = new QVBoxLayout( this ); 
    m_color_chooserLayout->setSpacing( 6 );
    m_color_chooserLayout->setMargin( 11 );

    m_color_group = new QGroupBox( this, "m_color_group" );
    m_color_group->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, m_color_group->sizePolicy().hasHeightForWidth() ) );
    m_color_group->setMinimumSize( QSize( 390, 350 ) );
    m_color_group->setMaximumSize( QSize( 390, 350 ) );
    m_color_group->setTitle( tr( "Colors" ) );

    Frame6 = new QFrame( m_color_group, "Frame6" );
    Frame6->setGeometry( QRect( 11, 19, 368, 45 ) ); 
    Frame6->setFrameShape( QFrame::StyledPanel );
    Frame6->setFrameShadow( QFrame::Raised );
    Frame6Layout = new QHBoxLayout( Frame6 ); 
    Frame6Layout->setSpacing( 6 );
    Frame6Layout->setMargin( 11 );

    m_color = new QToolButton( Frame6, "m_color" );
    m_color->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, m_color->sizePolicy().hasHeightForWidth() ) );
    m_color->setMinimumSize( QSize( 20, 0 ) );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 127, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 63, 63) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 0, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 0, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 127, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 38, 38) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 0, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 0, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 127, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 38, 38) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 0, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 0, 0) );
    cg.setColor( QColorGroup::Text, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    m_color->setPalette( pal );
    m_color->setText( tr( "" ) );
    Frame6Layout->addWidget( m_color );

    m_text = new QLineEdit( Frame6, "m_text" );
    Frame6Layout->addWidget( m_text );

    ListBox3 = new QListBox( m_color_group, "ListBox3" );
    ListBox3->insertItem( tr( "New Item" ) );
    ListBox3->setGeometry( QRect( 80, 160, 221, 111 ) ); 
    m_color_chooserLayout->addWidget( m_color_group );

    Layout4 = new QHBoxLayout; 
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );

    Frame8 = new QFrame( this, "Frame8" );
    Frame8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, Frame8->sizePolicy().hasHeightForWidth() ) );
    Frame8->setMinimumSize( QSize( 0, 45 ) );
    Frame8->setFrameShape( QFrame::StyledPanel );
    Frame8->setFrameShadow( QFrame::Raised );
    Frame8Layout = new QHBoxLayout( Frame8 ); 
    Frame8Layout->setSpacing( 6 );
    Frame8Layout->setMargin( 11 );

    m_dimension_label = new QLabel( Frame8, "m_dimension_label" );
    m_dimension_label->setText( tr( "Dimension:" ) );
    Frame8Layout->addWidget( m_dimension_label );

    SpinBox1 = new QSpinBox( Frame8, "SpinBox1" );
    SpinBox1->setMaxValue( 10 );
    QToolTip::add(  SpinBox1, tr( "Set dimension. 0=AUTO" ) );
    Frame8Layout->addWidget( SpinBox1 );
    Layout4->addWidget( Frame8 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer );
    m_color_chooserLayout->addLayout( Layout4 );

    Line1 = new QFrame( this, "Line1" );
    Line1->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    m_color_chooserLayout->addWidget( Line1 );

    Layout2 = new QHBoxLayout; 
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    m_close_button = new QPushButton( this, "m_close_button" );
    m_close_button->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, m_close_button->sizePolicy().hasHeightForWidth() ) );
    m_close_button->setText( tr( "OK" ) );
    QToolTip::add(  m_close_button, tr( "" ) );
    Layout2->addWidget( m_close_button );

    m_cancel_button = new QPushButton( this, "m_cancel_button" );
    m_cancel_button->setText( tr( "Cancel" ) );
    Layout2->addWidget( m_cancel_button );

    m_add_button = new QPushButton( this, "m_add_button" );
    m_add_button->setText( tr( "Add Item" ) );
    Layout2->addWidget( m_add_button );
    m_color_chooserLayout->addLayout( Layout2 );

    // signals and slots connections
    connect( m_close_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
m_color_chooser::~m_color_chooser()
{
    // no need to delete child widgets, Qt does it all for us
}

