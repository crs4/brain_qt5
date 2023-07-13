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
** Form interface generated from reading ui file 'test.ui'
**
** Created: Sat Jun 16 12:13:55 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef M_COLOR_CHOOSER_H
#define M_COLOR_CHOOSER_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QFrame;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListBox;
class QListBoxItem;
class QPushButton;
class QSpinBox;
class QToolButton;

class m_color_chooser : public QDialog
{ 
    Q_OBJECT

public:
    m_color_chooser( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~m_color_chooser();

    QGroupBox* m_color_group;
    QFrame* Frame6;
    QToolButton* m_color;
    QLineEdit* m_text;
    QListBox* ListBox3;
    QFrame* Frame8;
    QLabel* m_dimension_label;
    QSpinBox* SpinBox1;
    QFrame* Line1;
    QPushButton* m_close_button;
    QPushButton* m_cancel_button;
    QPushButton* m_add_button;

protected:
    QVBoxLayout* m_color_chooserLayout;
    QHBoxLayout* Frame6Layout;
    QHBoxLayout* Layout4;
    QHBoxLayout* Frame8Layout;
    QHBoxLayout* Layout2;
};

#endif // M_COLOR_CHOOSER_H
