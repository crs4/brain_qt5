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
** Form generated from reading UI file 'bsViewer2DOptions.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BSVIEWER2DOPTIONS_H
#define UI_BSVIEWER2DOPTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_bsViewer2DOptions
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QHBoxLayout *hboxLayout1;
    QLabel *TextLabel2;
    QLineEdit *m_cell_stepX;
    QSpacerItem *Spacer2;
    QHBoxLayout *hboxLayout2;
    QHBoxLayout *hboxLayout3;
    QLabel *TextLabel2_2;
    QLineEdit *m_cell_stepY;
    QSpacerItem *Spacer3;
    QHBoxLayout *hboxLayout4;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QSpacerItem *Horizontal_Spacing2_2;

    void setupUi(QDialog *bsViewer2DOptions)
    {
        if (bsViewer2DOptions->objectName().isEmpty())
            bsViewer2DOptions->setObjectName(QString::fromUtf8("bsViewer2DOptions"));
        bsViewer2DOptions->setWindowModality(Qt::WindowModal);
        bsViewer2DOptions->resize(386, 181);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bsViewer2DOptions->sizePolicy().hasHeightForWidth());
        bsViewer2DOptions->setSizePolicy(sizePolicy);
        bsViewer2DOptions->setSizeGripEnabled(true);
        vboxLayout = new QVBoxLayout(bsViewer2DOptions);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        TextLabel2 = new QLabel(bsViewer2DOptions);
        TextLabel2->setObjectName(QString::fromUtf8("TextLabel2"));
        TextLabel2->setWordWrap(false);

        hboxLayout1->addWidget(TextLabel2);

        m_cell_stepX = new QLineEdit(bsViewer2DOptions);
        m_cell_stepX->setObjectName(QString::fromUtf8("m_cell_stepX"));

        hboxLayout1->addWidget(m_cell_stepX);


        hboxLayout->addLayout(hboxLayout1);

        Spacer2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer2);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        TextLabel2_2 = new QLabel(bsViewer2DOptions);
        TextLabel2_2->setObjectName(QString::fromUtf8("TextLabel2_2"));
        TextLabel2_2->setWordWrap(false);

        hboxLayout3->addWidget(TextLabel2_2);

        m_cell_stepY = new QLineEdit(bsViewer2DOptions);
        m_cell_stepY->setObjectName(QString::fromUtf8("m_cell_stepY"));

        hboxLayout3->addWidget(m_cell_stepY);


        hboxLayout2->addLayout(hboxLayout3);

        Spacer3 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(Spacer3);


        vboxLayout->addLayout(hboxLayout2);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout4->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(bsViewer2DOptions);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);

        hboxLayout4->addWidget(buttonOk);

        buttonCancel = new QPushButton(bsViewer2DOptions);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout4->addWidget(buttonCancel);

        Horizontal_Spacing2_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout4->addItem(Horizontal_Spacing2_2);


        vboxLayout->addLayout(hboxLayout4);


        retranslateUi(bsViewer2DOptions);
        QObject::connect(buttonOk, SIGNAL(clicked()), bsViewer2DOptions, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), bsViewer2DOptions, SLOT(reject()));

        buttonOk->setDefault(true);


        QMetaObject::connectSlotsByName(bsViewer2DOptions);
    } // setupUi

    void retranslateUi(QDialog *bsViewer2DOptions)
    {
        bsViewer2DOptions->setWindowTitle(QCoreApplication::translate("bsViewer2DOptions", "Viewer 2D Options", nullptr));
        TextLabel2->setText(QCoreApplication::translate("bsViewer2DOptions", "Cell step X:", nullptr));
        m_cell_stepX->setText(QCoreApplication::translate("bsViewer2DOptions", "10000", nullptr));
        TextLabel2_2->setText(QCoreApplication::translate("bsViewer2DOptions", "Cell step Y:", nullptr));
        m_cell_stepY->setText(QCoreApplication::translate("bsViewer2DOptions", "10000", nullptr));
        buttonOk->setText(QCoreApplication::translate("bsViewer2DOptions", "&OK", nullptr));
        buttonCancel->setText(QCoreApplication::translate("bsViewer2DOptions", "&Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bsViewer2DOptions: public Ui_bsViewer2DOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSVIEWER2DOPTIONS_H
