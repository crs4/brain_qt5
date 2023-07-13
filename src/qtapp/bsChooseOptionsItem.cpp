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
#include "bsChooseOptionsItem.h"

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qcolordialog.h>
#include <qspinbox.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <QCheckBox>

/* 
 *  Constructs a bsChooseOptionsItem which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
bsChooseOptionsItem::bsChooseOptionsItem( QWidget* parent, const bool show_flag)
    : QFrame( parent )
{
    m_show_flag=show_flag;
    setWindowTitle( tr( "Choose Option Item" ) );

    m_choose_itemLayout = new QHBoxLayout( this ); 
    m_choose_itemLayout->setSpacing( 6 );
    m_choose_itemLayout->setMargin( 11 );

    m_color = new QPushButton( this );
    //m_color->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, m_color->sizePolicy().hasHeightForWidth() ) );
    m_color->setMinimumSize( QSize( 20, 0 ) );
    m_color->setAutoFillBackground(true);
    connect( m_color, SIGNAL( clicked() ), this, SLOT( slotColor() ) );
    m_choose_itemLayout->addWidget( m_color );

    m_text = new QLineEdit( this );
    m_text->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_choose_itemLayout->addWidget( m_text );


    m_color_value = QColor(255,255,255);
    setColor(m_color_value);

    m_flag_aligned = new QCheckBox(this);
    {
      QFont m_flag_font(  m_flag_aligned->font() );
      m_flag_font.setPointSize( 10 );
      m_flag_aligned->setFont( m_flag_font ); 
      m_flag_aligned->setText( tr( "Aligned" ) );
      m_choose_itemLayout->addWidget( m_flag_aligned );
    }
    m_flag_enabled = new QCheckBox(this);
    {
      QFont m_flag_font(  m_flag_enabled->font() );
      m_flag_font.setPointSize( 10 );
      m_flag_enabled->setFont( m_flag_font ); 
      m_flag_enabled->setText( tr( "Enabled" ) );
      m_choose_itemLayout->addWidget( m_flag_enabled );
    }
    m_size = new QSpinBox(this);
    {
      m_size->setMinimum( 0 );
      m_size->setMaximum( 10 );
      m_size->setValue( 0 );
      m_choose_itemLayout->addWidget( m_size );
    }

    if (!m_show_flag) {
      m_flag_aligned->hide();
      m_flag_enabled->hide();
      m_size->hide();
    }
}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsChooseOptionsItem::~bsChooseOptionsItem()
{
    // no need to delete child widgets, Qt does it all for us
}

void bsChooseOptionsItem::setColor(const QColor c) {
  m_color->setText( tr( "" ) );
  QString bg;
  bg=QString::asprintf("background-color: rgb(%d, %d, %d)", c.red(), c.green(), c.blue());
  m_color->setStyleSheet(bg);
  m_color_value=c;
}

QColor bsChooseOptionsItem::getColor() const {
  return m_color_value;
}
  
void bsChooseOptionsItem::setText(const QString s) {
  m_text->setText(s);
}

QString bsChooseOptionsItem::getText() const { 
  return m_text->text();
}

void bsChooseOptionsItem::slotColor()
{
    QColor c = QColorDialog::getColor(m_color_value, this );
    if (c.isValid()) {
      setColor(c);
    }
}

void bsChooseOptionsItem::setFlagAligned(const bool b) {
  m_flag_aligned->setChecked(b);
}

bool bsChooseOptionsItem::getFlagAligned() const { 
  return m_flag_aligned->isChecked();
}

void bsChooseOptionsItem::setFlagEnabled(const bool b) {
  m_flag_enabled->setChecked(b);
}

bool bsChooseOptionsItem::getFlagEnabled() const { 
  return m_flag_enabled->isChecked();
}

void bsChooseOptionsItem::setSize(const int value) {
  if (value >= m_size->minimum() && value <= m_size->maximum()) {
    m_size->setValue(value);
  }
}

int bsChooseOptionsItem::getSize() const { 
  return m_size->value();
}



