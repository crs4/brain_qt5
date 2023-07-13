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
#include "bsMiniFastEditor3D.h"
#include "bsMiniViewerWindow3DImpl.h"

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
#include "Icons/trans3D.xpm"
#include "Icons/trans3D.xbm"

#include <iostream>


bsMiniFastEditor3D::bsMiniFastEditor3D( QWidget* parent, qtCommandHistory* cmd_processor)
    : bsEditor3D( parent, cmd_processor) {

  m_viewer_window = new bsMiniViewerWindow3DImpl(this, cmd_processor);

  m_tool_layout =  m_viewer_window->getToolLayout();
  m_tool_frame = m_viewer_window->getToolFrame();
  m_viewer = m_viewer_window->getViewer3D();

  QPixmap trans3DIcon = QPixmap(trans3DPixmap);
  //QBitmap trans3DMask(trans3D_width,trans3D_height,trans3D_bits, TRUE);
  QBitmap trans3DMask=QBitmap::fromData(QSize(trans3D_width,trans3D_height), trans3D_bits);
  

  insertTool(new bsTransform3DTool(this, m_tool_frame, "Transform3D", &trans3DIcon, &trans3DMask));

  enableAllTools(false);

  m_top_layout->addWidget(m_viewer_window);
  m_top_layout->activate();
}


bsMiniFastEditor3D::~bsMiniFastEditor3D() {
  //  delete m_viewer_window;
}

bsFastBrainViewer* bsMiniFastEditor3D::getFastBrainViewer() {
  return m_viewer_window->getViewer3D();
}

void  bsMiniFastEditor3D::setDocumentViewerWindow() {
  m_viewer_window->setDocument(m_document);
}

void bsMiniFastEditor3D::setEnableSubRegions(bool b){
  m_viewer_window->setEnableSubRegions(b);
}

void bsMiniFastEditor3D::setEnableNeuronTypes(bool b){
  m_viewer_window->setEnableNeuronTypes(b);
}

void bsMiniFastEditor3D::setEnableSelectedRenderer(bool b){
  m_viewer_window->setEnableSelectedRenderer(b);
}

void bsMiniFastEditor3D::setSelectedRendererIndex(int n){
  m_viewer_window->setSelectedRendererIndex(n);
}
