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
** Form generated from reading UI file 'bsIsoViewerWindow3D.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BSISOVIEWERWINDOW3D_H
#define UI_BSISOVIEWERWINDOW3D_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <q3frame.h>
#include "bsIsoBrainViewer.h"

QT_BEGIN_NAMESPACE

class Ui_bsIsoViewerWindow3D
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *m_reset_button;
    QPushButton *m_rebuild_button;
    QRadioButton *m_enable_subregions;
    QRadioButton *m_enable_neurons;
    QRadioButton *m_enable_cutting_planes;
    QSpacerItem *Spacer1;
    QLabel *TextLabel1;
    QSlider *m_extinction_zeta;
    Q3Frame *m_tool_frame;
    Q3Frame *bsViewerFrame;
    QVBoxLayout *vboxLayout1;
    bsIsoBrainViewer *m_viewer;

    void setupUi(QWidget *bsIsoViewerWindow3D)
    {
        if (bsIsoViewerWindow3D->objectName().isEmpty())
            bsIsoViewerWindow3D->setObjectName(QString::fromUtf8("bsIsoViewerWindow3D"));
        bsIsoViewerWindow3D->resize(638, 417);
        vboxLayout = new QVBoxLayout(bsIsoViewerWindow3D);
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
        m_reset_button = new QPushButton(bsIsoViewerWindow3D);
        m_reset_button->setObjectName(QString::fromUtf8("m_reset_button"));
        m_reset_button->setIcon(QPixmap(QString::fromUtf8("image0")));

        hboxLayout->addWidget(m_reset_button);

        m_rebuild_button = new QPushButton(bsIsoViewerWindow3D);
        m_rebuild_button->setObjectName(QString::fromUtf8("m_rebuild_button"));

        hboxLayout->addWidget(m_rebuild_button);

        m_enable_subregions = new QRadioButton(bsIsoViewerWindow3D);
        m_enable_subregions->setObjectName(QString::fromUtf8("m_enable_subregions"));
        QFont font;
        font.setPointSize(8);
        m_enable_subregions->setFont(font);

        hboxLayout->addWidget(m_enable_subregions);

        m_enable_neurons = new QRadioButton(bsIsoViewerWindow3D);
        m_enable_neurons->setObjectName(QString::fromUtf8("m_enable_neurons"));
        m_enable_neurons->setFont(font);

        hboxLayout->addWidget(m_enable_neurons);

        m_enable_cutting_planes = new QRadioButton(bsIsoViewerWindow3D);
        m_enable_cutting_planes->setObjectName(QString::fromUtf8("m_enable_cutting_planes"));
        m_enable_cutting_planes->setFont(font);

        hboxLayout->addWidget(m_enable_cutting_planes);

        Spacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Spacer1);

        TextLabel1 = new QLabel(bsIsoViewerWindow3D);
        TextLabel1->setObjectName(QString::fromUtf8("TextLabel1"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TextLabel1->sizePolicy().hasHeightForWidth());
        TextLabel1->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QString::fromUtf8("helvetica"));
        font1.setPointSize(8);
        TextLabel1->setFont(font1);
        TextLabel1->setWordWrap(false);

        hboxLayout->addWidget(TextLabel1);

        m_extinction_zeta = new QSlider(bsIsoViewerWindow3D);
        m_extinction_zeta->setObjectName(QString::fromUtf8("m_extinction_zeta"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_extinction_zeta->sizePolicy().hasHeightForWidth());
        m_extinction_zeta->setSizePolicy(sizePolicy1);
        m_extinction_zeta->setMaximumSize(QSize(100, 20));
        m_extinction_zeta->setFont(font1);
        m_extinction_zeta->setMaximum(30);
        m_extinction_zeta->setValue(10);
        m_extinction_zeta->setOrientation(Qt::Horizontal);
        m_extinction_zeta->setTickPosition(QSlider::TicksBothSides);
        m_extinction_zeta->setTickInterval(3);

        hboxLayout->addWidget(m_extinction_zeta);

        m_tool_frame = new Q3Frame(bsIsoViewerWindow3D);
        m_tool_frame->setObjectName(QString::fromUtf8("m_tool_frame"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_tool_frame->sizePolicy().hasHeightForWidth());
        m_tool_frame->setSizePolicy(sizePolicy2);
        m_tool_frame->setMinimumSize(QSize(40, 40));
        m_tool_frame->setMaximumSize(QSize(32767, 40));
        m_tool_frame->setFrameShape(QFrame::StyledPanel);
        m_tool_frame->setFrameShadow(QFrame::Raised);

        hboxLayout->addWidget(m_tool_frame);


        vboxLayout->addLayout(hboxLayout);

        bsViewerFrame = new Q3Frame(bsIsoViewerWindow3D);
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
        m_viewer = new bsIsoBrainViewer(bsViewerFrame);
        m_viewer->setObjectName(QString::fromUtf8("m_viewer"));

        vboxLayout1->addWidget(m_viewer);


        vboxLayout->addWidget(bsViewerFrame);


        retranslateUi(bsIsoViewerWindow3D);
        QObject::connect(m_enable_neurons, SIGNAL(toggled(bool)), m_viewer, SLOT(setEnableNeuronTypes(bool)));
        QObject::connect(m_enable_subregions, SIGNAL(toggled(bool)), m_viewer, SLOT(setEnableSubRegions(bool)));
        QObject::connect(m_rebuild_button, SIGNAL(clicked()), m_viewer, SLOT(rebuild()));
        QObject::connect(m_reset_button, SIGNAL(clicked()), m_viewer, SLOT(reset()));
        QObject::connect(m_extinction_zeta, SIGNAL(valueChanged(int)), m_viewer, SLOT(handleExtinctionZetaChange(int)));
        QObject::connect(m_enable_cutting_planes, SIGNAL(toggled(bool)), m_viewer, SLOT(setCuttingPlanesFlag(bool)));

        QMetaObject::connectSlotsByName(bsIsoViewerWindow3D);
    } // setupUi

    void retranslateUi(QWidget *bsIsoViewerWindow3D)
    {
        bsIsoViewerWindow3D->setWindowTitle(QCoreApplication::translate("bsIsoViewerWindow3D", "bsIsoViewerWindow3D", nullptr));
        m_reset_button->setText(QString());
#if QT_CONFIG(tooltip)
        m_reset_button->setToolTip(QCoreApplication::translate("bsIsoViewerWindow3D", "Reset", nullptr));
#endif // QT_CONFIG(tooltip)
        m_rebuild_button->setText(QCoreApplication::translate("bsIsoViewerWindow3D", "Rebuild", nullptr));
        m_enable_subregions->setText(QCoreApplication::translate("bsIsoViewerWindow3D", "Subregions", nullptr));
#if QT_CONFIG(tooltip)
        m_enable_subregions->setToolTip(QCoreApplication::translate("bsIsoViewerWindow3D", "Draft Quality", nullptr));
#endif // QT_CONFIG(tooltip)
        m_enable_neurons->setText(QCoreApplication::translate("bsIsoViewerWindow3D", "Neurons", nullptr));
#if QT_CONFIG(tooltip)
        m_enable_neurons->setToolTip(QCoreApplication::translate("bsIsoViewerWindow3D", "Draft Quality", nullptr));
#endif // QT_CONFIG(tooltip)
        m_enable_cutting_planes->setText(QCoreApplication::translate("bsIsoViewerWindow3D", "Cut", nullptr));
#if QT_CONFIG(tooltip)
        m_enable_cutting_planes->setToolTip(QCoreApplication::translate("bsIsoViewerWindow3D", "Draft Quality", nullptr));
#endif // QT_CONFIG(tooltip)
        TextLabel1->setText(QCoreApplication::translate("bsIsoViewerWindow3D", "Alpha", nullptr));
#if QT_CONFIG(tooltip)
        m_extinction_zeta->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        m_extinction_zeta->setWhatsThis(QCoreApplication::translate("bsIsoViewerWindow3D", "sets the transparency of neurons", nullptr));
#endif // QT_CONFIG(whatsthis)
    } // retranslateUi

};

namespace Ui {
    class bsIsoViewerWindow3D: public Ui_bsIsoViewerWindow3D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSISOVIEWERWINDOW3D_H
