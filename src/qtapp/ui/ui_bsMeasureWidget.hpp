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
/********************************************************************************
** Form generated from reading UI file 'bsMeasureWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BSMEASUREWIDGET_H
#define UI_BSMEASUREWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_bsMeasureWidget
{
public:
    QGridLayout *gridLayout;
    QFrame *Frame3;
    QGridLayout *gridLayout1;
    QLabel *m_label_01;
    QLineEdit *m_start_point;
    QLineEdit *m_end_point;
    QLabel *m_label_02;
    QLabel *m_label_03;
    QLineEdit *m_distance;

    void setupUi(QWidget *bsMeasureWidget)
    {
        if (bsMeasureWidget->objectName().isEmpty())
            bsMeasureWidget->setObjectName(QString::fromUtf8("bsMeasureWidget"));
        bsMeasureWidget->resize(600, 480);
        gridLayout = new QGridLayout(bsMeasureWidget);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        Frame3 = new QFrame(bsMeasureWidget);
        Frame3->setObjectName(QString::fromUtf8("Frame3"));
        Frame3->setFrameShadow(QFrame::Raised);
        Frame3->setFrameShape(QFrame::StyledPanel);
        gridLayout1 = new QGridLayout(Frame3);
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        m_label_01 = new QLabel(Frame3);
        m_label_01->setObjectName(QString::fromUtf8("m_label_01"));
        m_label_01->setWordWrap(false);

        gridLayout1->addWidget(m_label_01, 0, 0, 1, 1);

        m_start_point = new QLineEdit(Frame3);
        m_start_point->setObjectName(QString::fromUtf8("m_start_point"));
        m_start_point->setFocusPolicy(Qt::NoFocus);
        m_start_point->setEnabled(true);

        gridLayout1->addWidget(m_start_point, 0, 1, 1, 1);

        m_end_point = new QLineEdit(Frame3);
        m_end_point->setObjectName(QString::fromUtf8("m_end_point"));
        m_end_point->setFocusPolicy(Qt::NoFocus);

        gridLayout1->addWidget(m_end_point, 1, 1, 1, 1);

        m_label_02 = new QLabel(Frame3);
        m_label_02->setObjectName(QString::fromUtf8("m_label_02"));
        m_label_02->setWordWrap(false);

        gridLayout1->addWidget(m_label_02, 1, 0, 1, 1);

        m_label_03 = new QLabel(Frame3);
        m_label_03->setObjectName(QString::fromUtf8("m_label_03"));
        m_label_03->setWordWrap(false);

        gridLayout1->addWidget(m_label_03, 2, 0, 1, 1);

        m_distance = new QLineEdit(Frame3);
        m_distance->setObjectName(QString::fromUtf8("m_distance"));
        m_distance->setFocusPolicy(Qt::NoFocus);

        gridLayout1->addWidget(m_distance, 2, 1, 1, 1);


        gridLayout->addWidget(Frame3, 0, 0, 1, 1);


        retranslateUi(bsMeasureWidget);

        QMetaObject::connectSlotsByName(bsMeasureWidget);
    } // setupUi

    void retranslateUi(QWidget *bsMeasureWidget)
    {
        bsMeasureWidget->setWindowTitle(QCoreApplication::translate("bsMeasureWidget", "Measure Tool", nullptr));
        m_label_01->setText(QCoreApplication::translate("bsMeasureWidget", "Start point", nullptr));
        m_start_point->setText(QCoreApplication::translate("bsMeasureWidget", "NULL", nullptr));
        m_end_point->setText(QCoreApplication::translate("bsMeasureWidget", "NULL", nullptr));
        m_label_02->setText(QCoreApplication::translate("bsMeasureWidget", "End point", nullptr));
        m_label_03->setText(QCoreApplication::translate("bsMeasureWidget", "Distance", nullptr));
        m_distance->setText(QCoreApplication::translate("bsMeasureWidget", "NULL", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bsMeasureWidget: public Ui_bsMeasureWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSMEASUREWIDGET_H
