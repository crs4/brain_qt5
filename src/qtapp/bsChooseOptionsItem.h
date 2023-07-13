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
#ifndef BSCHOOSEOPTIONSITEM_H
#define BSCHOOSEOPTIONSITEM_H

#include <qvariant.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFrame>

class QLineEdit;
class QPushButton;
class QColor;
class QCheckBox;
class QSpinBox;

class bsChooseOptionsItem : public QFrame
{ 
    Q_OBJECT

public:
    bsChooseOptionsItem( QWidget* parent = 0, const bool show_flag=false);
    ~bsChooseOptionsItem();

    void setColor(const QColor c);
    QColor getColor() const;
  
    void setText(const QString s);
    QString getText() const;

    void setFlagAligned(const bool b);
    bool getFlagAligned() const;

    void setFlagEnabled(const bool b);
    bool getFlagEnabled() const;

    void setSize(const int value);
    int getSize() const;

    QPushButton* m_color;
    QLineEdit* m_text;
    QCheckBox* m_flag_aligned;
    QCheckBox* m_flag_enabled;
    QSpinBox* m_size;

signals:
    void toggled(bool);

protected slots:
    void slotColor();

protected:
    QHBoxLayout* m_choose_itemLayout;
    QColor m_color_value;
    bool m_show_flag;
};

#endif // BSCHOOSEOPTIONSITEM_H
