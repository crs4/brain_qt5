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
** Form generated from reading UI file 'bsCuttingBoxWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BSCUTTINGBOXWIDGET_H
#define UI_BSCUTTINGBOXWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_bsCuttingBoxWidget
{
public:
    QHBoxLayout *hboxLayout;
    QFrame *Frame3;
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout1;
    QLabel *m_label_i;
    QSlider *m_slider_i;
    QHBoxLayout *hboxLayout2;
    QLabel *m_label_j;
    QSlider *m_slider_j;
    QHBoxLayout *hboxLayout3;
    QLabel *m_label_k;
    QSlider *m_slider_k;
    QGridLayout *gridLayout1;
    QLabel *m_label_02;
    QLabel *m_label_03;
    QLineEdit *m_l;
    QLineEdit *m_h;
    QLabel *m_label_01;
    QLineEdit *m_w;
    QPushButton *m_reset_button;
    QFrame *Line4;
    QFrame *Line5;

    void setupUi(QWidget *bsCuttingBoxWidget)
    {
        if (bsCuttingBoxWidget->objectName().isEmpty())
            bsCuttingBoxWidget->setObjectName(QString::fromUtf8("bsCuttingBoxWidget"));
        bsCuttingBoxWidget->resize(384, 195);
        hboxLayout = new QHBoxLayout(bsCuttingBoxWidget);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        Frame3 = new QFrame(bsCuttingBoxWidget);
        Frame3->setObjectName(QString::fromUtf8("Frame3"));
        Frame3->setFrameShape(QFrame::StyledPanel);
        Frame3->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(Frame3);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_label_i = new QLabel(Frame3);
        m_label_i->setObjectName(QString::fromUtf8("m_label_i"));
        m_label_i->setMinimumSize(QSize(20, 0));
        m_label_i->setAlignment(Qt::AlignCenter);
        m_label_i->setWordWrap(false);

        hboxLayout1->addWidget(m_label_i);

        m_slider_i = new QSlider(Frame3);
        m_slider_i->setObjectName(QString::fromUtf8("m_slider_i"));
        m_slider_i->setMinimum(-100);
        m_slider_i->setMaximum(100);
        m_slider_i->setOrientation(Qt::Horizontal);

        hboxLayout1->addWidget(m_slider_i);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout2->setSpacing(6);
#endif
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        m_label_j = new QLabel(Frame3);
        m_label_j->setObjectName(QString::fromUtf8("m_label_j"));
        m_label_j->setMinimumSize(QSize(20, 0));
        m_label_j->setAlignment(Qt::AlignCenter);
        m_label_j->setWordWrap(false);

        hboxLayout2->addWidget(m_label_j);

        m_slider_j = new QSlider(Frame3);
        m_slider_j->setObjectName(QString::fromUtf8("m_slider_j"));
        m_slider_j->setMinimum(-100);
        m_slider_j->setMaximum(100);
        m_slider_j->setOrientation(Qt::Horizontal);

        hboxLayout2->addWidget(m_slider_j);


        vboxLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout3->setSpacing(6);
#endif
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        m_label_k = new QLabel(Frame3);
        m_label_k->setObjectName(QString::fromUtf8("m_label_k"));
        m_label_k->setMinimumSize(QSize(20, 0));
        m_label_k->setAlignment(Qt::AlignCenter);
        m_label_k->setWordWrap(false);

        hboxLayout3->addWidget(m_label_k);

        m_slider_k = new QSlider(Frame3);
        m_slider_k->setObjectName(QString::fromUtf8("m_slider_k"));
        m_slider_k->setMinimum(-100);
        m_slider_k->setMaximum(100);
        m_slider_k->setOrientation(Qt::Horizontal);

        hboxLayout3->addWidget(m_slider_k);


        vboxLayout->addLayout(hboxLayout3);


        gridLayout->addLayout(vboxLayout, 0, 2, 1, 1);

        gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        m_label_02 = new QLabel(Frame3);
        m_label_02->setObjectName(QString::fromUtf8("m_label_02"));
        m_label_02->setWordWrap(false);

        gridLayout1->addWidget(m_label_02, 1, 0, 1, 1);

        m_label_03 = new QLabel(Frame3);
        m_label_03->setObjectName(QString::fromUtf8("m_label_03"));
        m_label_03->setWordWrap(false);

        gridLayout1->addWidget(m_label_03, 2, 0, 1, 1);

        m_l = new QLineEdit(Frame3);
        m_l->setObjectName(QString::fromUtf8("m_l"));
        m_l->setFocusPolicy(Qt::ClickFocus);

        gridLayout1->addWidget(m_l, 1, 1, 1, 1);

        m_h = new QLineEdit(Frame3);
        m_h->setObjectName(QString::fromUtf8("m_h"));
        m_h->setFocusPolicy(Qt::ClickFocus);

        gridLayout1->addWidget(m_h, 2, 1, 1, 1);

        m_label_01 = new QLabel(Frame3);
        m_label_01->setObjectName(QString::fromUtf8("m_label_01"));
        m_label_01->setWordWrap(false);

        gridLayout1->addWidget(m_label_01, 0, 0, 1, 1);

        m_w = new QLineEdit(Frame3);
        m_w->setObjectName(QString::fromUtf8("m_w"));
        m_w->setEnabled(true);
        m_w->setFocusPolicy(Qt::ClickFocus);

        gridLayout1->addWidget(m_w, 0, 1, 1, 1);


        gridLayout->addLayout(gridLayout1, 0, 0, 1, 1);

        m_reset_button = new QPushButton(Frame3);
        m_reset_button->setObjectName(QString::fromUtf8("m_reset_button"));

        gridLayout->addWidget(m_reset_button, 2, 0, 1, 1);

        Line4 = new QFrame(Frame3);
        Line4->setObjectName(QString::fromUtf8("Line4"));

        gridLayout->addWidget(Line4, 1, 0, 1, 4);

        Line5 = new QFrame(Frame3);
        Line5->setObjectName(QString::fromUtf8("Line5"));

        gridLayout->addWidget(Line5, 0, 1, 1, 1);


        hboxLayout->addWidget(Frame3);


        retranslateUi(bsCuttingBoxWidget);

        QMetaObject::connectSlotsByName(bsCuttingBoxWidget);
    } // setupUi

    void retranslateUi(QWidget *bsCuttingBoxWidget)
    {
        bsCuttingBoxWidget->setWindowTitle(QCoreApplication::translate("bsCuttingBoxWidget", "Cutting Box Tool", nullptr));
        m_label_i->setText(QCoreApplication::translate("bsCuttingBoxWidget", "I", nullptr));
        m_label_j->setText(QCoreApplication::translate("bsCuttingBoxWidget", "J", nullptr));
        m_label_k->setText(QCoreApplication::translate("bsCuttingBoxWidget", "K", nullptr));
        m_label_02->setText(QCoreApplication::translate("bsCuttingBoxWidget", "L", nullptr));
        m_label_03->setText(QCoreApplication::translate("bsCuttingBoxWidget", "H", nullptr));
        m_l->setText(QCoreApplication::translate("bsCuttingBoxWidget", "NULL", nullptr));
        m_h->setText(QCoreApplication::translate("bsCuttingBoxWidget", "NULL", nullptr));
        m_label_01->setText(QCoreApplication::translate("bsCuttingBoxWidget", "W", nullptr));
        m_w->setText(QCoreApplication::translate("bsCuttingBoxWidget", "NULL", nullptr));
        m_reset_button->setText(QCoreApplication::translate("bsCuttingBoxWidget", "Reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bsCuttingBoxWidget: public Ui_bsCuttingBoxWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSCUTTINGBOXWIDGET_H
