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
** Form generated from reading UI file 'bsViewerWindow3D.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BSVIEWERWINDOW3D_H
#define UI_BSVIEWERWINDOW3D_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "bsFastBrainViewer.h"

QT_BEGIN_NAMESPACE

class Ui_bsViewerWindow3D
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QGridLayout *gridLayout;
    QPushButton *m_reset_top;
    QPushButton *m_reset_bottom;
    QPushButton *m_reset_right;
    QPushButton *m_reset_rear;
    QPushButton *m_reset_button;
    QPushButton *m_reset_left;
    QRadioButton *m_draft_quality_button;
    QRadioButton *m_enable_subregions;
    QRadioButton *m_enable_neurons;
    QRadioButton *m_enable_cutting_planes;
    QSpacerItem *Spacer1;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout1;
    QLabel *TextLabel1;
    QSpinBox *m_extinction_zeta;
    QHBoxLayout *hboxLayout2;
    QLabel *TextLabel1_2_3;
    QSpinBox *m_extinction_zeta_threshold_start;
    QHBoxLayout *hboxLayout3;
    QLabel *TextLabel1_2_2_2;
    QSpinBox *m_extinction_zeta_threshold_end;
    QFrame *m_tool_frame;
    QFrame *bsViewerFrame;
    QVBoxLayout *vboxLayout1;
    bsFastBrainViewer *m_viewer;

    void setupUi(QWidget *bsViewerWindow3D)
    {
        if (bsViewerWindow3D->objectName().isEmpty())
            bsViewerWindow3D->setObjectName(QString::fromUtf8("bsViewerWindow3D"));
        bsViewerWindow3D->resize(847, 693);
        vboxLayout = new QVBoxLayout(bsViewerWindow3D);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_reset_top = new QPushButton(bsViewerWindow3D);
        m_reset_top->setObjectName(QString::fromUtf8("m_reset_top"));
        m_reset_top->setMinimumSize(QSize(50, 25));
        m_reset_top->setMaximumSize(QSize(50, 25));
        QFont font;
        font.setFamily(QString::fromUtf8("adobe-helvetica"));
        font.setPointSize(8);
        m_reset_top->setFont(font);

        gridLayout->addWidget(m_reset_top, 0, 2, 1, 1);

        m_reset_bottom = new QPushButton(bsViewerWindow3D);
        m_reset_bottom->setObjectName(QString::fromUtf8("m_reset_bottom"));
        m_reset_bottom->setMinimumSize(QSize(50, 25));
        m_reset_bottom->setMaximumSize(QSize(50, 25));
        m_reset_bottom->setFont(font);

        gridLayout->addWidget(m_reset_bottom, 1, 2, 1, 1);

        m_reset_right = new QPushButton(bsViewerWindow3D);
        m_reset_right->setObjectName(QString::fromUtf8("m_reset_right"));
        m_reset_right->setMinimumSize(QSize(50, 25));
        m_reset_right->setMaximumSize(QSize(50, 25));
        m_reset_right->setFont(font);

        gridLayout->addWidget(m_reset_right, 0, 1, 1, 1);

        m_reset_rear = new QPushButton(bsViewerWindow3D);
        m_reset_rear->setObjectName(QString::fromUtf8("m_reset_rear"));
        m_reset_rear->setMinimumSize(QSize(40, 25));
        m_reset_rear->setMaximumSize(QSize(40, 25));
        m_reset_rear->setFont(font);

        gridLayout->addWidget(m_reset_rear, 1, 0, 1, 1);

        m_reset_button = new QPushButton(bsViewerWindow3D);
        m_reset_button->setObjectName(QString::fromUtf8("m_reset_button"));
        m_reset_button->setIcon(QPixmap(QString::fromUtf8("image0")));

        gridLayout->addWidget(m_reset_button, 0, 0, 1, 1);

        m_reset_left = new QPushButton(bsViewerWindow3D);
        m_reset_left->setObjectName(QString::fromUtf8("m_reset_left"));
        m_reset_left->setMinimumSize(QSize(50, 25));
        m_reset_left->setMaximumSize(QSize(50, 25));
        m_reset_left->setFont(font);

        gridLayout->addWidget(m_reset_left, 1, 1, 1, 1);


        hboxLayout->addLayout(gridLayout);

        m_draft_quality_button = new QRadioButton(bsViewerWindow3D);
        m_draft_quality_button->setObjectName(QString::fromUtf8("m_draft_quality_button"));
        QFont font1;
        font1.setPointSize(8);
        m_draft_quality_button->setFont(font1);

        hboxLayout->addWidget(m_draft_quality_button);

        m_enable_subregions = new QRadioButton(bsViewerWindow3D);
        m_enable_subregions->setObjectName(QString::fromUtf8("m_enable_subregions"));
        m_enable_subregions->setFont(font1);

        hboxLayout->addWidget(m_enable_subregions);

        m_enable_neurons = new QRadioButton(bsViewerWindow3D);
        m_enable_neurons->setObjectName(QString::fromUtf8("m_enable_neurons"));
        m_enable_neurons->setFont(font1);

        hboxLayout->addWidget(m_enable_neurons);

        m_enable_cutting_planes = new QRadioButton(bsViewerWindow3D);
        m_enable_cutting_planes->setObjectName(QString::fromUtf8("m_enable_cutting_planes"));
        m_enable_cutting_planes->setFont(font1);

        hboxLayout->addWidget(m_enable_cutting_planes);

        Spacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer1);

        gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(0);
#ifndef Q_OS_MAC
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        TextLabel1 = new QLabel(bsViewerWindow3D);
        TextLabel1->setObjectName(QString::fromUtf8("TextLabel1"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TextLabel1->sizePolicy().hasHeightForWidth());
        TextLabel1->setSizePolicy(sizePolicy);
        TextLabel1->setMinimumSize(QSize(30, 0));
        QFont font2;
        font2.setFamily(QString::fromUtf8("helvetica"));
        font2.setPointSize(8);
        TextLabel1->setFont(font2);
        TextLabel1->setWordWrap(false);

        hboxLayout1->addWidget(TextLabel1);

        m_extinction_zeta = new QSpinBox(bsViewerWindow3D);
        m_extinction_zeta->setObjectName(QString::fromUtf8("m_extinction_zeta"));
        m_extinction_zeta->setMinimumSize(QSize(80, 0));
        m_extinction_zeta->setMaximum(100);
        m_extinction_zeta->setMinimum(1);
        m_extinction_zeta->setValue(1);

        hboxLayout1->addWidget(m_extinction_zeta);


        gridLayout1->addLayout(hboxLayout1, 0, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(0);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        TextLabel1_2_3 = new QLabel(bsViewerWindow3D);
        TextLabel1_2_3->setObjectName(QString::fromUtf8("TextLabel1_2_3"));
        sizePolicy.setHeightForWidth(TextLabel1_2_3->sizePolicy().hasHeightForWidth());
        TextLabel1_2_3->setSizePolicy(sizePolicy);
        TextLabel1_2_3->setMinimumSize(QSize(30, 0));
        TextLabel1_2_3->setFont(font2);
        TextLabel1_2_3->setWordWrap(false);

        hboxLayout2->addWidget(TextLabel1_2_3);

        m_extinction_zeta_threshold_start = new QSpinBox(bsViewerWindow3D);
        m_extinction_zeta_threshold_start->setObjectName(QString::fromUtf8("m_extinction_zeta_threshold_start"));
        m_extinction_zeta_threshold_start->setMinimumSize(QSize(80, 0));
        m_extinction_zeta_threshold_start->setMaximum(10000);

        hboxLayout2->addWidget(m_extinction_zeta_threshold_start);


        gridLayout1->addLayout(hboxLayout2, 1, 0, 1, 1);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(0);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        TextLabel1_2_2_2 = new QLabel(bsViewerWindow3D);
        TextLabel1_2_2_2->setObjectName(QString::fromUtf8("TextLabel1_2_2_2"));
        sizePolicy.setHeightForWidth(TextLabel1_2_2_2->sizePolicy().hasHeightForWidth());
        TextLabel1_2_2_2->setSizePolicy(sizePolicy);
        TextLabel1_2_2_2->setMinimumSize(QSize(30, 0));
        TextLabel1_2_2_2->setFont(font2);
        TextLabel1_2_2_2->setWordWrap(false);

        hboxLayout3->addWidget(TextLabel1_2_2_2);

        m_extinction_zeta_threshold_end = new QSpinBox(bsViewerWindow3D);
        m_extinction_zeta_threshold_end->setObjectName(QString::fromUtf8("m_extinction_zeta_threshold_end"));
        m_extinction_zeta_threshold_end->setMaximum(10000);
        m_extinction_zeta_threshold_end->setMinimum(1);
        m_extinction_zeta_threshold_end->setValue(2000);

        hboxLayout3->addWidget(m_extinction_zeta_threshold_end);


        gridLayout1->addLayout(hboxLayout3, 1, 1, 1, 1);


        hboxLayout->addLayout(gridLayout1);

        m_tool_frame = new QFrame(bsViewerWindow3D);
        m_tool_frame->setObjectName(QString::fromUtf8("m_tool_frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_tool_frame->sizePolicy().hasHeightForWidth());
        m_tool_frame->setSizePolicy(sizePolicy1);
        m_tool_frame->setMinimumSize(QSize(40, 40));
        m_tool_frame->setMaximumSize(QSize(32767, 40));
        m_tool_frame->setFrameShape(QFrame::StyledPanel);
        m_tool_frame->setFrameShadow(QFrame::Raised);

        hboxLayout->addWidget(m_tool_frame);


        vboxLayout->addLayout(hboxLayout);

        bsViewerFrame = new QFrame(bsViewerWindow3D);
        bsViewerFrame->setObjectName(QString::fromUtf8("bsViewerFrame"));
        QPalette palette;
        palette.setColor(QPalette::Active, QPalette::WindowText, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::Button, QColor(160, 160, 160));
        palette.setColor(QPalette::Active, QPalette::Light, QColor(240, 240, 240));
        palette.setColor(QPalette::Active, QPalette::Midlight, QColor(200, 200, 200));
        palette.setColor(QPalette::Active, QPalette::Dark, QColor(80, 80, 80));
        palette.setColor(QPalette::Active, QPalette::Mid, QColor(106, 106, 106));
        palette.setColor(QPalette::Active, QPalette::Text, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::BrightText, QColor(255, 255, 255));
        palette.setColor(QPalette::Active, QPalette::ButtonText, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::Base, QColor(196, 196, 196));
        palette.setColor(QPalette::Active, QPalette::Window, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::Shadow, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::Highlight, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::HighlightedText, QColor(196, 196, 196));
        palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::Button, QColor(160, 160, 160));
        palette.setColor(QPalette::Inactive, QPalette::Light, QColor(240, 240, 240));
        palette.setColor(QPalette::Inactive, QPalette::Midlight, QColor(184, 184, 184));
        palette.setColor(QPalette::Inactive, QPalette::Dark, QColor(80, 80, 80));
        palette.setColor(QPalette::Inactive, QPalette::Mid, QColor(106, 106, 106));
        palette.setColor(QPalette::Inactive, QPalette::Text, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::BrightText, QColor(255, 255, 255));
        palette.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::Base, QColor(196, 196, 196));
        palette.setColor(QPalette::Inactive, QPalette::Window, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::Shadow, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::Highlight, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(196, 196, 196));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128, 128, 128));
        palette.setColor(QPalette::Disabled, QPalette::Button, QColor(160, 160, 160));
        palette.setColor(QPalette::Disabled, QPalette::Light, QColor(240, 240, 240));
        palette.setColor(QPalette::Disabled, QPalette::Midlight, QColor(184, 184, 184));
        palette.setColor(QPalette::Disabled, QPalette::Dark, QColor(80, 80, 80));
        palette.setColor(QPalette::Disabled, QPalette::Mid, QColor(106, 106, 106));
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(0, 0, 0));
        palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255, 255, 255));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128, 128, 128));
        palette.setColor(QPalette::Disabled, QPalette::Base, QColor(196, 196, 196));
        palette.setColor(QPalette::Disabled, QPalette::Window, QColor(0, 0, 0));
        palette.setColor(QPalette::Disabled, QPalette::Shadow, QColor(0, 0, 0));
        palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(0, 0, 0));
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(196, 196, 196));
        bsViewerFrame->setPalette(palette);
        bsViewerFrame->setFrameShape(QFrame::StyledPanel);
        bsViewerFrame->setFrameShadow(QFrame::Raised);
        vboxLayout1 = new QVBoxLayout(bsViewerFrame);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
        vboxLayout1->setContentsMargins(11, 11, 11, 11);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        m_viewer = new bsFastBrainViewer(bsViewerFrame);
        m_viewer->setObjectName(QString::fromUtf8("m_viewer"));

        vboxLayout1->addWidget(m_viewer);


        vboxLayout->addWidget(bsViewerFrame);


        retranslateUi(bsViewerWindow3D);
        QObject::connect(m_draft_quality_button, SIGNAL(toggled(bool)), m_viewer, SLOT(setDraftQuality(bool)));
        QObject::connect(m_enable_subregions, SIGNAL(toggled(bool)), m_viewer, SLOT(setEnableSubRegions(bool)));
        QObject::connect(m_enable_neurons, SIGNAL(toggled(bool)), m_viewer, SLOT(setEnableNeuronTypes(bool)));
        QObject::connect(m_enable_cutting_planes, SIGNAL(toggled(bool)), m_viewer, SLOT(setCuttingPlanesFlag(bool)));
        QObject::connect(m_extinction_zeta_threshold_start, SIGNAL(valueChanged(int)), m_viewer, SLOT(handleExtinctionZetaThresholdChangeStart(int)));
        QObject::connect(m_extinction_zeta_threshold_end, SIGNAL(valueChanged(int)), m_viewer, SLOT(handleExtinctionZetaThresholdChangeEnd(int)));
        QObject::connect(m_extinction_zeta, SIGNAL(valueChanged(int)), m_viewer, SLOT(handleExtinctionZetaChange(int)));
        QObject::connect(m_reset_button, SIGNAL(clicked()), m_viewer, SLOT(reset()));
        QObject::connect(m_reset_rear, SIGNAL(clicked()), m_viewer, SLOT(reset_rear()));
        QObject::connect(m_reset_right, SIGNAL(clicked()), m_viewer, SLOT(reset_right()));
        QObject::connect(m_reset_left, SIGNAL(clicked()), m_viewer, SLOT(reset_left()));
        QObject::connect(m_reset_top, SIGNAL(clicked()), m_viewer, SLOT(reset_top()));
        QObject::connect(m_reset_bottom, SIGNAL(clicked()), m_viewer, SLOT(reset_bottom()));

        QMetaObject::connectSlotsByName(bsViewerWindow3D);
    } // setupUi

    void retranslateUi(QWidget *bsViewerWindow3D)
    {
        bsViewerWindow3D->setWindowTitle(QCoreApplication::translate("bsViewerWindow3D", "bsViewerWindow3D", nullptr));
        bsViewerWindow3D->setWindowIconText(QString());
        m_reset_top->setText(QCoreApplication::translate("bsViewerWindow3D", "Top", nullptr));
#if QT_CONFIG(tooltip)
        m_reset_top->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Reset", nullptr));
#endif // QT_CONFIG(tooltip)
        m_reset_bottom->setText(QCoreApplication::translate("bsViewerWindow3D", "Bottom", nullptr));
#if QT_CONFIG(tooltip)
        m_reset_bottom->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Reset", nullptr));
#endif // QT_CONFIG(tooltip)
        m_reset_right->setText(QCoreApplication::translate("bsViewerWindow3D", "Right", nullptr));
#if QT_CONFIG(tooltip)
        m_reset_right->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Reset", nullptr));
#endif // QT_CONFIG(tooltip)
        m_reset_rear->setText(QCoreApplication::translate("bsViewerWindow3D", "Rear", nullptr));
#if QT_CONFIG(tooltip)
        m_reset_rear->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Reset", nullptr));
#endif // QT_CONFIG(tooltip)
        m_reset_button->setText(QString());
#if QT_CONFIG(tooltip)
        m_reset_button->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Reset", nullptr));
#endif // QT_CONFIG(tooltip)
        m_reset_left->setText(QCoreApplication::translate("bsViewerWindow3D", "Left", nullptr));
#if QT_CONFIG(tooltip)
        m_reset_left->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Reset", nullptr));
#endif // QT_CONFIG(tooltip)
        m_draft_quality_button->setText(QCoreApplication::translate("bsViewerWindow3D", "Draft", nullptr));
#if QT_CONFIG(tooltip)
        m_draft_quality_button->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Draft Quality", nullptr));
#endif // QT_CONFIG(tooltip)
        m_enable_subregions->setText(QCoreApplication::translate("bsViewerWindow3D", "Subregions", nullptr));
#if QT_CONFIG(tooltip)
        m_enable_subregions->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Display subregions", nullptr));
#endif // QT_CONFIG(tooltip)
        m_enable_neurons->setText(QCoreApplication::translate("bsViewerWindow3D", "Neurons", nullptr));
#if QT_CONFIG(tooltip)
        m_enable_neurons->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Display neurons", nullptr));
#endif // QT_CONFIG(tooltip)
        m_enable_cutting_planes->setText(QCoreApplication::translate("bsViewerWindow3D", "Cut", nullptr));
#if QT_CONFIG(tooltip)
        m_enable_cutting_planes->setToolTip(QCoreApplication::translate("bsViewerWindow3D", "Enable cutting planes", nullptr));
#endif // QT_CONFIG(tooltip)
        TextLabel1->setText(QCoreApplication::translate("bsViewerWindow3D", "Alpha", nullptr));
        TextLabel1_2_3->setText(QCoreApplication::translate("bsViewerWindow3D", "Start", nullptr));
        TextLabel1_2_2_2->setText(QCoreApplication::translate("bsViewerWindow3D", "End", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bsViewerWindow3D: public Ui_bsViewerWindow3D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSVIEWERWINDOW3D_H
