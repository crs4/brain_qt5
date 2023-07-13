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
** Form generated from reading UI file 'bsMiniViewerWindow3D.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BSMINIVIEWERWINDOW3D_H
#define UI_BSMINIVIEWERWINDOW3D_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "bsFastBrainViewer.h"

QT_BEGIN_NAMESPACE

class Ui_bsMiniViewerWindow3D
{
public:
    QGridLayout *gridLayout;
    QFrame *bsViewerFrame;
    QVBoxLayout *vboxLayout;
    bsFastBrainViewer *m_viewer;
    QGridLayout *gridLayout1;
    QFrame *m_tool_frame;
    QPushButton *m_reset_button;
    QSpacerItem *Spacer1;
    QRadioButton *m_draft_quality_button;

    void setupUi(QWidget *bsMiniViewerWindow3D)
    {
        if (bsMiniViewerWindow3D->objectName().isEmpty())
            bsMiniViewerWindow3D->setObjectName(QString::fromUtf8("bsMiniViewerWindow3D"));
        bsMiniViewerWindow3D->resize(247, 313);
        gridLayout = new QGridLayout(bsMiniViewerWindow3D);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bsViewerFrame = new QFrame(bsMiniViewerWindow3D);
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
        vboxLayout = new QVBoxLayout(bsViewerFrame);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        m_viewer = new bsFastBrainViewer(bsViewerFrame);
        m_viewer->setObjectName(QString::fromUtf8("m_viewer"));

        vboxLayout->addWidget(m_viewer);


        gridLayout->addWidget(bsViewerFrame, 1, 0, 1, 1);

        gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        m_tool_frame = new QFrame(bsMiniViewerWindow3D);
        m_tool_frame->setObjectName(QString::fromUtf8("m_tool_frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_tool_frame->sizePolicy().hasHeightForWidth());
        m_tool_frame->setSizePolicy(sizePolicy);
        m_tool_frame->setMinimumSize(QSize(40, 40));
        m_tool_frame->setMaximumSize(QSize(32767, 40));
        m_tool_frame->setFrameShape(QFrame::StyledPanel);
        m_tool_frame->setFrameShadow(QFrame::Raised);

        gridLayout1->addWidget(m_tool_frame, 0, 3, 1, 1);

        m_reset_button = new QPushButton(bsMiniViewerWindow3D);
        m_reset_button->setObjectName(QString::fromUtf8("m_reset_button"));
        m_reset_button->setIcon(QPixmap(QString::fromUtf8("image0")));

        gridLayout1->addWidget(m_reset_button, 0, 0, 1, 1);

        Spacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(Spacer1, 0, 2, 1, 1);

        m_draft_quality_button = new QRadioButton(bsMiniViewerWindow3D);
        m_draft_quality_button->setObjectName(QString::fromUtf8("m_draft_quality_button"));
        QFont font;
        font.setPointSize(8);
        m_draft_quality_button->setFont(font);

        gridLayout1->addWidget(m_draft_quality_button, 0, 1, 1, 1);


        gridLayout->addLayout(gridLayout1, 0, 0, 1, 1);


        retranslateUi(bsMiniViewerWindow3D);
        QObject::connect(m_reset_button, SIGNAL(clicked()), m_viewer, SLOT(reset()));
        QObject::connect(m_draft_quality_button, SIGNAL(toggled(bool)), m_viewer, SLOT(setDraftQuality(bool)));

        QMetaObject::connectSlotsByName(bsMiniViewerWindow3D);
    } // setupUi

    void retranslateUi(QWidget *bsMiniViewerWindow3D)
    {
        bsMiniViewerWindow3D->setWindowTitle(QCoreApplication::translate("bsMiniViewerWindow3D", "bsMiniViewerWindow3D", nullptr));
        m_reset_button->setText(QString());
#if QT_CONFIG(tooltip)
        m_reset_button->setToolTip(QCoreApplication::translate("bsMiniViewerWindow3D", "Reset", nullptr));
#endif // QT_CONFIG(tooltip)
        m_draft_quality_button->setText(QCoreApplication::translate("bsMiniViewerWindow3D", "Draft", nullptr));
#if QT_CONFIG(tooltip)
        m_draft_quality_button->setToolTip(QCoreApplication::translate("bsMiniViewerWindow3D", "Draft Quality", nullptr));
#endif // QT_CONFIG(tooltip)
    } // retranslateUi

};

namespace Ui {
    class bsMiniViewerWindow3D: public Ui_bsMiniViewerWindow3D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSMINIVIEWERWINDOW3D_H
