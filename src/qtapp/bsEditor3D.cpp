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
#include "bsEditor3D.h"
#include "bsDocument.h"
#include "bsBrainViewer.h"

#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QVBoxLayout>
#include <QBoxLayout>

#include <assert.h>

#include <qpoint.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qcursor.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qbitmap.h>

#include <iostream>

bsEditor3D::bsEditor3D( QWidget* parent, qtCommandHistory* cmd_processor)
    : bsEditor( parent, cmd_processor) {

  m_tool_layout = NULL;
  m_tool_frame = NULL;
  m_top_layout = new QVBoxLayout(this);
  m_viewer = NULL;
}


bsEditor3D::~bsEditor3D() {
}


void  bsEditor3D::setDocument(bsDocument* doc){
    bsEditor::setDocument(doc);
    connect(doc,SIGNAL(newBrain2D(bool)),SLOT(resetAllTools()));
}

QBoxLayout* bsEditor3D::getToolLayout() {
  return m_tool_layout;
}

QFrame* bsEditor3D::getToolFrame() {
  return m_tool_frame;
}

bsViewer* bsEditor3D::getViewer() {
  return m_viewer;
}

bsViewer3D* bsEditor3D::getViewer3D() {
  return m_viewer;
}

bsBrainViewer* bsEditor3D::getBrainViewer() {
  return m_viewer;
}

