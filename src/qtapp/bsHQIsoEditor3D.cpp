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
#include "bsHQIsoEditor3D.h"
#include "bsHQIsoViewerWindow3DImpl.h"

#include "bsDocument.h"

#include "qtCommandHistory.h"

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

#include "bsTransform3DTool.h"
#include "bsCuttingBoxTool.h"

#include "Icons/trans3D.xpm"
#include "Icons/trans3D.xbm"

#include "Icons/cut3D.xpm"
#include "Icons/cut3D.xbm"

#include <iostream>


bsHQIsoEditor3D::bsHQIsoEditor3D( QWidget* parent, qtCommandHistory* cmd_processor)
    : bsEditor3D( parent, cmd_processor) {

  m_viewer_window = new bsHQIsoViewerWindow3DImpl(this, cmd_processor);

  m_tool_layout =  m_viewer_window->getToolLayout();
  m_tool_frame = m_viewer_window->getToolFrame();
  m_viewer = m_viewer_window->getViewer3D();

  QPixmap cut3DIcon = QPixmap(cut3DPixmap);
  //QBitmap cut3DMask(cut3D_width,cut3D_height,cut3D_bits, TRUE);
  QBitmap cut3DMask=QBitmap::fromData(QSize(cut3D_width,cut3D_height), cut3D_bits);

  m_cutting_box_tool = new bsCuttingBoxTool(this, m_tool_frame, "Cut", &cut3DIcon, &cut3DMask);
  insertTool(m_cutting_box_tool);

  QPixmap trans3DIcon = QPixmap(trans3DPixmap);
  //QBitmap trans3DMask(trans3D_width,trans3D_height,trans3D_bits, TRUE);
  QBitmap trans3DMask=QBitmap::fromData(QSize(trans3D_width,trans3D_height), trans3D_bits);
  insertTool(new bsTransform3DTool(this, m_tool_frame, "Transform3D", &trans3DIcon, &trans3DMask));

  enableAllTools(false);

  m_top_layout->addWidget(m_viewer_window);
  m_top_layout->activate();
}


bsHQIsoEditor3D::~bsHQIsoEditor3D() {
  //  delete m_viewer_window;
}

bsHQIsoBrainViewer* bsHQIsoEditor3D::getIsoBrainViewer() {
  return m_viewer_window->getViewer3D();
}

void  bsHQIsoEditor3D::setDocumentViewerWindow() {
  m_viewer_window->setDocument(m_document);
}

void bsHQIsoEditor3D::setEnableSubRegions(bool b){
  m_viewer_window->setEnableSubRegions(b);
}

void bsHQIsoEditor3D::setEnableNeuronTypes(bool b){
  m_viewer_window->setEnableNeuronTypes(b);
}

void bsHQIsoEditor3D::setDocument(bsDocument* doc){
  bsEditor3D::setDocument(doc);
  connect(m_document, SIGNAL(newCuttingBoxLoaded()), m_cutting_box_tool, SLOT(updateCuttingBoxWindow()));
}
