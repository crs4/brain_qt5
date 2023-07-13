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
** Form generated from reading UI file 'bsViewerWindow2D.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BSVIEWERWINDOW2D_H
#define UI_BSVIEWERWINDOW2D_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "bsSectionViewer2D.h"

QT_BEGIN_NAMESPACE

class Ui_bsViewerWindow2D
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout;
    QRadioButton *enable_mask;
    QSpinBox *spin_th;
    QSlider *m_x_slider;
    QHBoxLayout *hboxLayout1;
    QSpinBox *m_zoom_slider;
    QCheckBox *m_grid;
    QPushButton *m_reset_button;
    QHBoxLayout *hboxLayout2;
    QSpinBox *currentSection;
    QSpinBox *upSection;
    QSpinBox *downSection;
    QGridLayout *gridLayout2;
    QRadioButton *m_image_button;
    QRadioButton *m_cut_button;
    QRadioButton *m_ghost_button;
    QRadioButton *m_fill_button;
    QLabel *m_error_info;
    QLabel *m_quota_lb;
    QLabel *m_quota_label;
    QFrame *m_tool_frame;
    QSpacerItem *Spacer1_2;
    QHBoxLayout *hboxLayout3;
    QFrame *bsViewerFrame;
    QVBoxLayout *vboxLayout;
    bsSectionViewer2D *m_viewer;
    QVBoxLayout *vboxLayout1;
    QSlider *m_y_slider;
    QPushButton *m_option_button;

    void setupUi(QWidget *bsViewerWindow2D)
    {
        if (bsViewerWindow2D->objectName().isEmpty())
            bsViewerWindow2D->setObjectName(QString::fromUtf8("bsViewerWindow2D"));
        bsViewerWindow2D->resize(810, 611);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bsViewerWindow2D->sizePolicy().hasHeightForWidth());
        bsViewerWindow2D->setSizePolicy(sizePolicy);
        bsViewerWindow2D->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(bsViewerWindow2D);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout1 = new QGridLayout();
        gridLayout1->setSpacing(6);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        enable_mask = new QRadioButton(bsViewerWindow2D);
        enable_mask->setObjectName(QString::fromUtf8("enable_mask"));
        enable_mask->setEnabled(false);

        hboxLayout->addWidget(enable_mask);

        spin_th = new QSpinBox(bsViewerWindow2D);
        spin_th->setObjectName(QString::fromUtf8("spin_th"));
        spin_th->setEnabled(false);
        spin_th->setMaximum(255);
        spin_th->setValue(210);

        hboxLayout->addWidget(spin_th);

        m_x_slider = new QSlider(bsViewerWindow2D);
        m_x_slider->setObjectName(QString::fromUtf8("m_x_slider"));
        m_x_slider->setMinimum(-100);
        m_x_slider->setMaximum(100);
        m_x_slider->setOrientation(Qt::Horizontal);

        hboxLayout->addWidget(m_x_slider);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        m_zoom_slider = new QSpinBox(bsViewerWindow2D);
        m_zoom_slider->setObjectName(QString::fromUtf8("m_zoom_slider"));
        m_zoom_slider->setMaximum(500);
        m_zoom_slider->setMinimum(5);
        m_zoom_slider->setSingleStep(5);
        m_zoom_slider->setValue(100);

        hboxLayout1->addWidget(m_zoom_slider);

        m_grid = new QCheckBox(bsViewerWindow2D);
        m_grid->setObjectName(QString::fromUtf8("m_grid"));
        QFont font;
        font.setFamily(QString::fromUtf8("adobe-helvetica"));
        font.setPointSize(8);
        m_grid->setFont(font);

        hboxLayout1->addWidget(m_grid);

        m_reset_button = new QPushButton(bsViewerWindow2D);
        m_reset_button->setObjectName(QString::fromUtf8("m_reset_button"));
        m_reset_button->setIcon(QPixmap(QString::fromUtf8("image0")));

        hboxLayout1->addWidget(m_reset_button);


        hboxLayout->addLayout(hboxLayout1);


        gridLayout1->addLayout(hboxLayout, 2, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        currentSection = new QSpinBox(bsViewerWindow2D);
        currentSection->setObjectName(QString::fromUtf8("currentSection"));
        currentSection->setMaximum(200);

        hboxLayout2->addWidget(currentSection);

        upSection = new QSpinBox(bsViewerWindow2D);
        upSection->setObjectName(QString::fromUtf8("upSection"));
        upSection->setMaximum(2);

        hboxLayout2->addWidget(upSection);

        downSection = new QSpinBox(bsViewerWindow2D);
        downSection->setObjectName(QString::fromUtf8("downSection"));
        downSection->setMaximum(0);
        downSection->setMinimum(-2);

        hboxLayout2->addWidget(downSection);

        gridLayout2 = new QGridLayout();
        gridLayout2->setSpacing(6);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        m_image_button = new QRadioButton(bsViewerWindow2D);
        m_image_button->setObjectName(QString::fromUtf8("m_image_button"));
        QFont font1;
        m_image_button->setFont(font1);

        gridLayout2->addWidget(m_image_button, 1, 1, 1, 1);

        m_cut_button = new QRadioButton(bsViewerWindow2D);
        m_cut_button->setObjectName(QString::fromUtf8("m_cut_button"));
        m_cut_button->setFont(font1);

        gridLayout2->addWidget(m_cut_button, 1, 0, 1, 1);

        m_ghost_button = new QRadioButton(bsViewerWindow2D);
        m_ghost_button->setObjectName(QString::fromUtf8("m_ghost_button"));
        m_ghost_button->setFont(font1);

        gridLayout2->addWidget(m_ghost_button, 0, 1, 1, 1);

        m_fill_button = new QRadioButton(bsViewerWindow2D);
        m_fill_button->setObjectName(QString::fromUtf8("m_fill_button"));
        m_fill_button->setFont(font1);

        gridLayout2->addWidget(m_fill_button, 0, 0, 1, 1);


        hboxLayout2->addLayout(gridLayout2);

        m_error_info = new QLabel(bsViewerWindow2D);
        m_error_info->setObjectName(QString::fromUtf8("m_error_info"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_error_info->sizePolicy().hasHeightForWidth());
        m_error_info->setSizePolicy(sizePolicy1);
        m_error_info->setMinimumSize(QSize(50, 20));
        QPalette palette;
        palette.setColor(QPalette::Active, QPalette::WindowText, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::Button, QColor(214, 214, 214));
        palette.setColor(QPalette::Active, QPalette::Light, QColor(255, 255, 255));
        palette.setColor(QPalette::Active, QPalette::Midlight, QColor(234, 234, 234));
        palette.setColor(QPalette::Active, QPalette::Dark, QColor(107, 107, 107));
        palette.setColor(QPalette::Active, QPalette::Mid, QColor(142, 142, 142));
        palette.setColor(QPalette::Active, QPalette::Text, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::BrightText, QColor(255, 255, 255));
        palette.setColor(QPalette::Active, QPalette::ButtonText, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::Base, QColor(255, 255, 255));
        palette.setColor(QPalette::Active, QPalette::Window, QColor(243, 243, 243));
        palette.setColor(QPalette::Active, QPalette::Shadow, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::Highlight, QColor(0, 0, 156));
        palette.setColor(QPalette::Active, QPalette::HighlightedText, QColor(255, 255, 255));
        palette.setColor(QPalette::Active, QPalette::Link, QColor(0, 0, 0));
        palette.setColor(QPalette::Active, QPalette::LinkVisited, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::Button, QColor(214, 214, 214));
        palette.setColor(QPalette::Inactive, QPalette::Light, QColor(255, 255, 255));
        palette.setColor(QPalette::Inactive, QPalette::Midlight, QColor(246, 246, 246));
        palette.setColor(QPalette::Inactive, QPalette::Dark, QColor(107, 107, 107));
        palette.setColor(QPalette::Inactive, QPalette::Mid, QColor(142, 142, 142));
        palette.setColor(QPalette::Inactive, QPalette::Text, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::BrightText, QColor(255, 255, 255));
        palette.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::Base, QColor(255, 255, 255));
        palette.setColor(QPalette::Inactive, QPalette::Window, QColor(243, 243, 243));
        palette.setColor(QPalette::Inactive, QPalette::Shadow, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::Highlight, QColor(0, 0, 156));
        palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(255, 255, 255));
        palette.setColor(QPalette::Inactive, QPalette::Link, QColor(0, 0, 0));
        palette.setColor(QPalette::Inactive, QPalette::LinkVisited, QColor(0, 0, 0));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128, 128, 128));
        palette.setColor(QPalette::Disabled, QPalette::Button, QColor(214, 214, 214));
        palette.setColor(QPalette::Disabled, QPalette::Light, QColor(255, 255, 255));
        palette.setColor(QPalette::Disabled, QPalette::Midlight, QColor(246, 246, 246));
        palette.setColor(QPalette::Disabled, QPalette::Dark, QColor(107, 107, 107));
        palette.setColor(QPalette::Disabled, QPalette::Mid, QColor(142, 142, 142));
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(0, 0, 0));
        palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255, 255, 255));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128, 128, 128));
        palette.setColor(QPalette::Disabled, QPalette::Base, QColor(255, 255, 255));
        palette.setColor(QPalette::Disabled, QPalette::Window, QColor(243, 243, 243));
        palette.setColor(QPalette::Disabled, QPalette::Shadow, QColor(0, 0, 0));
        palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(0, 0, 156));
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(255, 255, 255));
        palette.setColor(QPalette::Disabled, QPalette::Link, QColor(0, 0, 0));
        palette.setColor(QPalette::Disabled, QPalette::LinkVisited, QColor(0, 0, 0));
        m_error_info->setPalette(palette);
        QFont font2;
        font2.setFamily(QString::fromUtf8("adobe-helvetica"));
        m_error_info->setFont(font2);
        m_error_info->setWordWrap(false);

        hboxLayout2->addWidget(m_error_info);

        m_quota_lb = new QLabel(bsViewerWindow2D);
        m_quota_lb->setObjectName(QString::fromUtf8("m_quota_lb"));
        m_quota_lb->setFont(font1);
        m_quota_lb->setWordWrap(false);

        hboxLayout2->addWidget(m_quota_lb);

        m_quota_label = new QLabel(bsViewerWindow2D);
        m_quota_label->setObjectName(QString::fromUtf8("m_quota_label"));
        sizePolicy1.setHeightForWidth(m_quota_label->sizePolicy().hasHeightForWidth());
        m_quota_label->setSizePolicy(sizePolicy1);
        m_quota_label->setMinimumSize(QSize(50, 20));
        QPalette palette1;
        palette1.setColor(QPalette::Active, QPalette::WindowText, QColor(0, 0, 0));
        palette1.setColor(QPalette::Active, QPalette::Button, QColor(214, 214, 214));
        palette1.setColor(QPalette::Active, QPalette::Light, QColor(255, 255, 255));
        palette1.setColor(QPalette::Active, QPalette::Midlight, QColor(234, 234, 234));
        palette1.setColor(QPalette::Active, QPalette::Dark, QColor(107, 107, 107));
        palette1.setColor(QPalette::Active, QPalette::Mid, QColor(142, 142, 142));
        palette1.setColor(QPalette::Active, QPalette::Text, QColor(0, 0, 0));
        palette1.setColor(QPalette::Active, QPalette::BrightText, QColor(255, 255, 255));
        palette1.setColor(QPalette::Active, QPalette::ButtonText, QColor(0, 0, 0));
        palette1.setColor(QPalette::Active, QPalette::Base, QColor(255, 255, 255));
        palette1.setColor(QPalette::Active, QPalette::Window, QColor(243, 243, 243));
        palette1.setColor(QPalette::Active, QPalette::Shadow, QColor(0, 0, 0));
        palette1.setColor(QPalette::Active, QPalette::Highlight, QColor(0, 0, 156));
        palette1.setColor(QPalette::Active, QPalette::HighlightedText, QColor(255, 255, 255));
        palette1.setColor(QPalette::Active, QPalette::Link, QColor(0, 0, 0));
        palette1.setColor(QPalette::Active, QPalette::LinkVisited, QColor(0, 0, 0));
        palette1.setColor(QPalette::Inactive, QPalette::WindowText, QColor(0, 0, 0));
        palette1.setColor(QPalette::Inactive, QPalette::Button, QColor(214, 214, 214));
        palette1.setColor(QPalette::Inactive, QPalette::Light, QColor(255, 255, 255));
        palette1.setColor(QPalette::Inactive, QPalette::Midlight, QColor(246, 246, 246));
        palette1.setColor(QPalette::Inactive, QPalette::Dark, QColor(107, 107, 107));
        palette1.setColor(QPalette::Inactive, QPalette::Mid, QColor(142, 142, 142));
        palette1.setColor(QPalette::Inactive, QPalette::Text, QColor(0, 0, 0));
        palette1.setColor(QPalette::Inactive, QPalette::BrightText, QColor(255, 255, 255));
        palette1.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(0, 0, 0));
        palette1.setColor(QPalette::Inactive, QPalette::Base, QColor(255, 255, 255));
        palette1.setColor(QPalette::Inactive, QPalette::Window, QColor(243, 243, 243));
        palette1.setColor(QPalette::Inactive, QPalette::Shadow, QColor(0, 0, 0));
        palette1.setColor(QPalette::Inactive, QPalette::Highlight, QColor(0, 0, 156));
        palette1.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(255, 255, 255));
        palette1.setColor(QPalette::Inactive, QPalette::Link, QColor(0, 0, 0));
        palette1.setColor(QPalette::Inactive, QPalette::LinkVisited, QColor(0, 0, 0));
        palette1.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128, 128, 128));
        palette1.setColor(QPalette::Disabled, QPalette::Button, QColor(214, 214, 214));
        palette1.setColor(QPalette::Disabled, QPalette::Light, QColor(255, 255, 255));
        palette1.setColor(QPalette::Disabled, QPalette::Midlight, QColor(246, 246, 246));
        palette1.setColor(QPalette::Disabled, QPalette::Dark, QColor(107, 107, 107));
        palette1.setColor(QPalette::Disabled, QPalette::Mid, QColor(142, 142, 142));
        palette1.setColor(QPalette::Disabled, QPalette::Text, QColor(0, 0, 0));
        palette1.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255, 255, 255));
        palette1.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128, 128, 128));
        palette1.setColor(QPalette::Disabled, QPalette::Base, QColor(255, 255, 255));
        palette1.setColor(QPalette::Disabled, QPalette::Window, QColor(243, 243, 243));
        palette1.setColor(QPalette::Disabled, QPalette::Shadow, QColor(0, 0, 0));
        palette1.setColor(QPalette::Disabled, QPalette::Highlight, QColor(0, 0, 156));
        palette1.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(255, 255, 255));
        palette1.setColor(QPalette::Disabled, QPalette::Link, QColor(0, 0, 0));
        palette1.setColor(QPalette::Disabled, QPalette::LinkVisited, QColor(0, 0, 0));
        m_quota_label->setPalette(palette1);
        m_quota_label->setWordWrap(false);

        hboxLayout2->addWidget(m_quota_label);

        m_tool_frame = new QFrame(bsViewerWindow2D);
        m_tool_frame->setObjectName(QString::fromUtf8("m_tool_frame"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_tool_frame->sizePolicy().hasHeightForWidth());
        m_tool_frame->setSizePolicy(sizePolicy2);
        m_tool_frame->setMinimumSize(QSize(60, 20));
        m_tool_frame->setFrameShape(QFrame::StyledPanel);
        m_tool_frame->setFrameShadow(QFrame::Raised);

        hboxLayout2->addWidget(m_tool_frame);

        Spacer1_2 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(Spacer1_2);


        gridLayout1->addLayout(hboxLayout2, 0, 0, 1, 1);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        bsViewerFrame = new QFrame(bsViewerWindow2D);
        bsViewerFrame->setObjectName(QString::fromUtf8("bsViewerFrame"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(bsViewerFrame->sizePolicy().hasHeightForWidth());
        bsViewerFrame->setSizePolicy(sizePolicy3);
        bsViewerFrame->setMinimumSize(QSize(600, 400));
        QPalette palette2;
        palette2.setColor(QPalette::Active, QPalette::WindowText, QColor(0, 0, 0));
        palette2.setColor(QPalette::Active, QPalette::Button, QColor(160, 160, 160));
        palette2.setColor(QPalette::Active, QPalette::Light, QColor(240, 240, 240));
        palette2.setColor(QPalette::Active, QPalette::Midlight, QColor(200, 200, 200));
        palette2.setColor(QPalette::Active, QPalette::Dark, QColor(80, 80, 80));
        palette2.setColor(QPalette::Active, QPalette::Mid, QColor(106, 106, 106));
        palette2.setColor(QPalette::Active, QPalette::Text, QColor(0, 0, 0));
        palette2.setColor(QPalette::Active, QPalette::BrightText, QColor(255, 255, 255));
        palette2.setColor(QPalette::Active, QPalette::ButtonText, QColor(0, 0, 0));
        palette2.setColor(QPalette::Active, QPalette::Base, QColor(196, 196, 196));
        palette2.setColor(QPalette::Active, QPalette::Window, QColor(0, 0, 0));
        palette2.setColor(QPalette::Active, QPalette::Shadow, QColor(0, 0, 0));
        palette2.setColor(QPalette::Active, QPalette::Highlight, QColor(0, 0, 0));
        palette2.setColor(QPalette::Active, QPalette::HighlightedText, QColor(196, 196, 196));
        palette2.setColor(QPalette::Active, QPalette::Link, QColor(0, 0, 0));
        palette2.setColor(QPalette::Active, QPalette::LinkVisited, QColor(0, 0, 0));
        palette2.setColor(QPalette::Inactive, QPalette::WindowText, QColor(0, 0, 0));
        palette2.setColor(QPalette::Inactive, QPalette::Button, QColor(160, 160, 160));
        palette2.setColor(QPalette::Inactive, QPalette::Light, QColor(240, 240, 240));
        palette2.setColor(QPalette::Inactive, QPalette::Midlight, QColor(184, 184, 184));
        palette2.setColor(QPalette::Inactive, QPalette::Dark, QColor(80, 80, 80));
        palette2.setColor(QPalette::Inactive, QPalette::Mid, QColor(106, 106, 106));
        palette2.setColor(QPalette::Inactive, QPalette::Text, QColor(0, 0, 0));
        palette2.setColor(QPalette::Inactive, QPalette::BrightText, QColor(255, 255, 255));
        palette2.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(0, 0, 0));
        palette2.setColor(QPalette::Inactive, QPalette::Base, QColor(196, 196, 196));
        palette2.setColor(QPalette::Inactive, QPalette::Window, QColor(0, 0, 0));
        palette2.setColor(QPalette::Inactive, QPalette::Shadow, QColor(0, 0, 0));
        palette2.setColor(QPalette::Inactive, QPalette::Highlight, QColor(0, 0, 0));
        palette2.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(196, 196, 196));
        palette2.setColor(QPalette::Inactive, QPalette::Link, QColor(0, 0, 0));
        palette2.setColor(QPalette::Inactive, QPalette::LinkVisited, QColor(0, 0, 0));
        palette2.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128, 128, 128));
        palette2.setColor(QPalette::Disabled, QPalette::Button, QColor(160, 160, 160));
        palette2.setColor(QPalette::Disabled, QPalette::Light, QColor(240, 240, 240));
        palette2.setColor(QPalette::Disabled, QPalette::Midlight, QColor(184, 184, 184));
        palette2.setColor(QPalette::Disabled, QPalette::Dark, QColor(80, 80, 80));
        palette2.setColor(QPalette::Disabled, QPalette::Mid, QColor(106, 106, 106));
        palette2.setColor(QPalette::Disabled, QPalette::Text, QColor(0, 0, 0));
        palette2.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255, 255, 255));
        palette2.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128, 128, 128));
        palette2.setColor(QPalette::Disabled, QPalette::Base, QColor(196, 196, 196));
        palette2.setColor(QPalette::Disabled, QPalette::Window, QColor(0, 0, 0));
        palette2.setColor(QPalette::Disabled, QPalette::Shadow, QColor(0, 0, 0));
        palette2.setColor(QPalette::Disabled, QPalette::Highlight, QColor(0, 0, 0));
        palette2.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(196, 196, 196));
        palette2.setColor(QPalette::Disabled, QPalette::Link, QColor(0, 0, 0));
        palette2.setColor(QPalette::Disabled, QPalette::LinkVisited, QColor(0, 0, 0));
        bsViewerFrame->setPalette(palette2);
        bsViewerFrame->setFrameShape(QFrame::StyledPanel);
        bsViewerFrame->setFrameShadow(QFrame::Raised);
        vboxLayout = new QVBoxLayout(bsViewerFrame);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        m_viewer = new bsSectionViewer2D(bsViewerFrame);
        m_viewer->setObjectName(QString::fromUtf8("m_viewer"));

        vboxLayout->addWidget(m_viewer);


        hboxLayout3->addWidget(bsViewerFrame);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        m_y_slider = new QSlider(bsViewerWindow2D);
        m_y_slider->setObjectName(QString::fromUtf8("m_y_slider"));
        m_y_slider->setMinimum(-100);
        m_y_slider->setMaximum(100);
        m_y_slider->setOrientation(Qt::Vertical);

        vboxLayout1->addWidget(m_y_slider);

        m_option_button = new QPushButton(bsViewerWindow2D);
        m_option_button->setObjectName(QString::fromUtf8("m_option_button"));
        sizePolicy1.setHeightForWidth(m_option_button->sizePolicy().hasHeightForWidth());
        m_option_button->setSizePolicy(sizePolicy1);
        m_option_button->setMinimumSize(QSize(34, 34));
        m_option_button->setMaximumSize(QSize(34, 34));
        m_option_button->setFont(font);

        vboxLayout1->addWidget(m_option_button);


        hboxLayout3->addLayout(vboxLayout1);


        gridLayout1->addLayout(hboxLayout3, 1, 0, 1, 1);


        gridLayout->addLayout(gridLayout1, 0, 0, 1, 1);


        retranslateUi(bsViewerWindow2D);
        QObject::connect(m_x_slider, SIGNAL(valueChanged(int)), m_viewer, SLOT(setXTranslation(int)));
        QObject::connect(m_y_slider, SIGNAL(valueChanged(int)), m_viewer, SLOT(setYTranslation(int)));
        QObject::connect(m_zoom_slider, SIGNAL(valueChanged(int)), m_viewer, SLOT(setZoomFactor(int)));
        QObject::connect(m_reset_button, SIGNAL(clicked()), m_viewer, SLOT(reset()));
        QObject::connect(currentSection, SIGNAL(valueChanged(int)), m_viewer, SLOT(setCurrentSection(int)));
        QObject::connect(m_viewer, SIGNAL(xTranslationChanged(int)), m_x_slider, SLOT(setValue(int)));
        QObject::connect(m_viewer, SIGNAL(yTranslationChanged(int)), m_y_slider, SLOT(setValue(int)));
        QObject::connect(m_viewer, SIGNAL(zoomFactorChanged(int)), m_zoom_slider, SLOT(setValue(int)));
        QObject::connect(m_viewer, SIGNAL(currentSectionChanged(int)), currentSection, SLOT(setValue(int)));
        QObject::connect(upSection, SIGNAL(valueChanged(int)), m_viewer, SLOT(setUpCountSection(int)));
        QObject::connect(downSection, SIGNAL(valueChanged(int)), m_viewer, SLOT(setDownCountSection(int)));
        QObject::connect(m_viewer, SIGNAL(enableUpCounter(bool)), upSection, SLOT(setEnabled(bool)));
        QObject::connect(m_viewer, SIGNAL(upCountSectionChanged(int)), upSection, SLOT(setValue(int)));
        QObject::connect(m_viewer, SIGNAL(downCountSectionChanged(int)), downSection, SLOT(setValue(int)));
        QObject::connect(m_viewer, SIGNAL(enableDownCounter(bool)), downSection, SLOT(setEnabled(bool)));
        QObject::connect(m_fill_button, SIGNAL(toggled(bool)), m_viewer, SLOT(fillSection(bool)));
        QObject::connect(m_cut_button, SIGNAL(toggled(bool)), m_viewer, SLOT(setCuttingPlanes(bool)));
        QObject::connect(m_grid, SIGNAL(toggled(bool)), m_viewer, SLOT(setGrid(bool)));
        QObject::connect(m_image_button, SIGNAL(toggled(bool)), m_viewer, SLOT(enableImageDrawing(bool)));
        QObject::connect(enable_mask, SIGNAL(toggled(bool)), m_viewer, SLOT(enableMask(bool)));
        QObject::connect(m_image_button, SIGNAL(toggled(bool)), enable_mask, SLOT(setEnabled(bool)));
        QObject::connect(spin_th, SIGNAL(valueChanged(int)), m_viewer, SLOT(setThreshold(int)));
        QObject::connect(m_image_button, SIGNAL(toggled(bool)), spin_th, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(bsViewerWindow2D);
    } // setupUi

    void retranslateUi(QWidget *bsViewerWindow2D)
    {
        bsViewerWindow2D->setWindowTitle(QCoreApplication::translate("bsViewerWindow2D", "bsViewerWindow2D", nullptr));
        enable_mask->setText(QCoreApplication::translate("bsViewerWindow2D", "mask", nullptr));
        m_zoom_slider->setSuffix(QCoreApplication::translate("bsViewerWindow2D", "%", nullptr));
        m_zoom_slider->setSpecialValueText(QString());
#if QT_CONFIG(tooltip)
        m_zoom_slider->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Zoom factor", nullptr)));
#endif // QT_CONFIG(tooltip)
        m_grid->setText(QCoreApplication::translate("bsViewerWindow2D", "Grid", nullptr));
        m_reset_button->setText(QString());
#if QT_CONFIG(tooltip)
        m_reset_button->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Reset", nullptr)));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        currentSection->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Current section", nullptr)));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        upSection->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Up counter", nullptr)));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        downSection->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Down counter", nullptr)));
#endif // QT_CONFIG(tooltip)
        m_image_button->setText(QCoreApplication::translate("bsViewerWindow2D", "Img", nullptr));
#if QT_CONFIG(tooltip)
        m_image_button->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Show bkg image", nullptr)));
#endif // QT_CONFIG(tooltip)
        m_cut_button->setText(QCoreApplication::translate("bsViewerWindow2D", "Cut", nullptr));
#if QT_CONFIG(tooltip)
        m_cut_button->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Cutting planes projection", nullptr)));
#endif // QT_CONFIG(tooltip)
        m_ghost_button->setText(QCoreApplication::translate("bsViewerWindow2D", "Ghost", nullptr));
#if QT_CONFIG(tooltip)
        m_ghost_button->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Enable ghost section", nullptr)));
#endif // QT_CONFIG(tooltip)
        m_fill_button->setText(QCoreApplication::translate("bsViewerWindow2D", "Fill", nullptr));
#if QT_CONFIG(tooltip)
        m_fill_button->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Fill section", nullptr)));
#endif // QT_CONFIG(tooltip)
        m_error_info->setText(QString());
#if QT_CONFIG(tooltip)
        m_error_info->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Auto-Intersection", nullptr)));
#endif // QT_CONFIG(tooltip)
        m_quota_lb->setText(QCoreApplication::translate("bsViewerWindow2D", "Quota:", nullptr));
        m_quota_label->setText(QString());
#if QT_CONFIG(tooltip)
        m_quota_label->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Quota level", nullptr)));
#endif // QT_CONFIG(tooltip)
        m_option_button->setText(QCoreApplication::translate("bsViewerWindow2D", "Opt.", nullptr));
#if QT_CONFIG(tooltip)
        m_option_button->setProperty("toolTip", QVariant(QCoreApplication::translate("bsViewerWindow2D", "Reset", nullptr)));
#endif // QT_CONFIG(tooltip)
    } // retranslateUi

};

namespace Ui {
    class bsViewerWindow2D: public Ui_bsViewerWindow2D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSVIEWERWINDOW2D_H
