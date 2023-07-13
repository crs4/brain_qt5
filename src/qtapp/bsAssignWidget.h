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
#ifndef bsAssignWidget_h
#define bsAssignWidget_h

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QListWidget>

class QPushButton;

class bsAssignWidget: public QDialog {
  Q_OBJECT
public:
    bsAssignWidget( QWidget* parent = 0 );
    ~bsAssignWidget();

protected slots:
  virtual void accept();
  virtual void reject();

public:
  void setItem(const char* label);
  void setCurrentItem(int id);

protected:

  QPushButton* m_ok_button;
  QPushButton* m_cancel_button;
  QListWidget* m_list_box;
  QVBoxLayout* vbox;
  QGridLayout* grid;
};

#endif
