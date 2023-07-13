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
#include "bsViewerWindow3DImpl.h"
#include "qtCommandHistory.h"

#include <assert.h>

#include <qframe.h>
#include <qpoint.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qcursor.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qbitmap.h>

#include "bsTransform3DTool.h"

#include "trans3D.xpm"
#include "trans3D.xbm"

#include <iostream>

bsEditor3D::bsEditor3D( QWidget* parent, const char* name,
				     WFlags fl, qtCommandHistory* cmd_processor)
    : bsEditor( parent, name, fl, cmd_processor) {

  QBoxLayout *topLayout = new QVBoxLayout(this);

  m_viewer_window = new bsViewerWindow3DImpl(this, 
					     "viewer window 3D", 
					     fl, cmd_processor);

  QBoxLayout* m_tool_layout = getToolLayout();
  QFrame* m_tool_frame = getToolFrame();

  QPixmap trans3DIcon = QPixmap(trans3DPixmap);
  QBitmap trans3DMask(trans3D_width,trans3D_height,trans3D_bits, TRUE);

  insertTool(new bsTransform3DTool(this, m_tool_frame, "Transform3D", &trans3DIcon, &trans3DMask));

  enableAllTools(false);

  topLayout->addWidget(m_viewer_window);
  topLayout->activate();
}


bsEditor3D::~bsEditor3D() {
  delete m_viewer_window;
}


bsViewer* bsEditor3D::getViewer() {
  return m_viewer_window->getViewer();
}

bsFastBrainViewer* bsEditor3D::getViewer3D() {
  return m_viewer_window->getViewer3D();
}

void  bsEditor3D::setDocumentViewerWindow() {
  m_viewer_window->setDocument(m_document);
}

void  bsEditor3D::setDocument(bsDocument* doc){
    bsEditor::setDocument(doc);
    connect(doc,SIGNAL(newBrain2D(bool)),SLOT(resetAllTools()));
}

QBoxLayout* bsEditor3D::getToolLayout() {
  return m_viewer_window->getToolLayout();
}

QFrame* bsEditor3D::getToolFrame() {
  return m_viewer_window->getToolFrame();
}

    
void bsEditor3D::setEnableSubRegions(bool b){
  m_viewer_window->setEnableSubRegions(b);
}

void bsEditor3D::setEnableNeuronTypes(bool b){
  m_viewer_window->setEnableNeuronTypes(b);
}


void bsEditor3D::setEnableSelectedRenderer(bool b){
  m_viewer_window->setEnableSelectedRenderer(b);
}

void bsEditor3D::setSelectedRendererIndex(int n){
  m_viewer_window->setSelectedRendererIndex(n);
}
