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
#ifndef BSCHOOSEOPTIONS_H
#define BSCHOOSEOPTIONS_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <vector>

class QLabel;
class QListBox;
class QListBoxItem;
class QPushButton;
class QSpinBox;
class QGroupBox;
class bsChooseOptionsItem;

class bsChooseOptions : public QDialog
{ 
    Q_OBJECT

public:
    bsChooseOptions( QWidget* parent = 0, bool show_flag=false);
    ~bsChooseOptions();

    void clear();
  
    float getDimension() const;
    float getDimension(const int i) const;

    void addItem(const QColor c, const QString s, const bool aligned=false, const bool enabled=false, const float dimension=0.0f);

    inline int itemCount() const {
      return m_list.size();
    }

    inline bool goodItemIndex(int i) const {
      return i>= 0 && i< itemCount();
    }

    QColor colorItem(const int i) const;
    QString textItem(const int i) const;
    bool flagAligned(const int i) const;
    bool flagEnabled(const int i) const;

public:
    QGroupBox* m_color_group_box;
  QVBoxLayout* m_color_group_box_layout;
    QFrame* m_dimension_frame;
    QLabel* m_dimension_label;
    QSpinBox* m_dimension;
    QFrame* Line1;
    QPushButton* m_close_button;
    QPushButton* m_cancel_button;
    QPushButton* m_add_button;
    std::vector<bsChooseOptionsItem*> m_list;

private slots:
      void addItem();
    
protected:
    QVBoxLayout* bsChooseOptionsLayout;
    QHBoxLayout* Layout4;
    QVBoxLayout* m_color_group_layout;
    QHBoxLayout* m_dimension_frameLayout;
    QHBoxLayout* m_buttons_layout;
    bool m_show_flag;
};

#endif // BSCHOOSEOPTIONS_H
