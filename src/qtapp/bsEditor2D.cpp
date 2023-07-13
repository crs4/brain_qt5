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
#include "bsEditor2D.h"
#include "bsDocument.h"
#include "bsViewerWindow2DImpl.h"
#include "qtCommandHistory.h"


#include "bsSectionRotateTool.h"
#include "bsSectionTranslateTool.h"
#include "bsAssignSubRegionTool.h"
#include "bsSectionDeformTool.h"
#include "bsMeasureTool.h"
#include "bsSectionEditTool.h"

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

#include "Icons/rotate.xpm"
#include "Icons/rotate.xbm"

#include "Icons/transxy.xpm"
#include "Icons/transxy.xbm"

#include "Icons/assignId.xpm"
#include "Icons/assignId.xbm"

#include "Icons/measure.xpm"
#include "Icons/measure.xbm"

#include "Icons/deform.xpm"
#include "Icons/deform.xbm"

#include "Icons/edit.xpm"
#include "Icons/edit.xbm"

#include <iostream>

bsEditor2D::bsEditor2D( QWidget* parent, qtCommandHistory* cmd_processor)
    : bsEditor( parent, cmd_processor) {

    QBoxLayout *topLayout = new QVBoxLayout(this);

    m_viewer_window = new bsViewerWindow2DImpl(this, cmd_processor);

    QFrame* m_tool_frame = getToolFrame();

    QPixmap transxyIcon = QPixmap(transxyPixmap);
    //QBitmap transxyMask(transxy_width,transxy_height,transxy_bits, true);
    QBitmap transxyMask=QBitmap::fromData(QSize(transxy_width,transxy_height), transxy_bits);
    insertTool(new bsSectionTranslateTool(this, m_tool_frame, "Translate XY", &transxyIcon, &transxyMask));

    QPixmap rotateIcon(rotatePixmap);
    //QBitmap rotateMask(rotate_width,rotate_height,rotate_bits, true);
    QBitmap rotateMask=QBitmap::fromData(QSize(rotate_width,rotate_height), rotate_bits);
    insertTool(new bsSectionRotateTool(this, m_tool_frame, "Rotate XY", &rotateIcon, &rotateMask));

    QPixmap assignIdIcon(assignIdPixmap);
    //QBitmap assignIdMask(assignId_width,assignId_height,assignId_bits, true);
    QBitmap assignIdMask=QBitmap::fromData(QSize(assignId_width,assignId_height), assignId_bits);
    insertTool(new bsAssignSubRegionTool(this, m_tool_frame, "Assign SubRegion Id", &assignIdIcon, &assignIdMask));

    QPixmap deformIcon(deformPixmap);
    //QBitmap deformMask(deform_width,deform_height,deform_bits, true);
    QBitmap deformMask=QBitmap::fromData(QSize(deform_width,deform_height), deform_bits);
    insertTool(new bsSectionDeformTool(this, m_tool_frame, "Deform Tool", &deformIcon, &deformMask));

    QPixmap measureIcon(measurePixmap);
    //QBitmap measureMask(measure_width,measure_height,measure_bits, true);
    QBitmap measureMask=QBitmap::fromData(QSize(measure_width,measure_height), measure_bits);
    insertTool(new bsMeasureTool(this, m_tool_frame, "Measure Tool", &measureIcon, &measureMask,C_BOTTOM_RIGHT));

    QPixmap editIcon(editPixmap);
    //QBitmap editMask(edit_width,edit_height,edit_bits, true);
    QBitmap editMask=QBitmap::fromData(QSize(edit_width,edit_height), edit_bits);
    insertTool(new bsSectionEditTool(this, m_tool_frame, "Edit Tool", &editIcon, &editMask, C_TOP_LEFT));

    enableAllTools(false);
    connect( m_viewer_window, SIGNAL( currentSectionChanged(int) ), this, SIGNAL( currentSectionChanged(int) ));
    
    topLayout->addWidget(m_viewer_window);
    topLayout->activate();
}


bsEditor2D::~bsEditor2D() {
}


bsViewer* bsEditor2D::getViewer() {
  return m_viewer_window->getViewer();
}


bsSectionViewer2D* bsEditor2D::getViewer2D() {
  return m_viewer_window->getViewer2D();
}

void  bsEditor2D::setDocumentViewerWindow() {
   m_viewer_window->setDocument(m_document);
}

void  bsEditor2D::setDocument(bsDocument* doc){
  bsEditor::setDocument(doc);
  connect(doc,SIGNAL(newBrain2D(bool)),SLOT(resetAllTools()));
}


int bsEditor2D::getCurrentSectionIndex() const{
  return m_viewer_window->getCurrentSectionIndex();
}

QBoxLayout* bsEditor2D::getToolLayout() {
  return m_viewer_window->getToolLayout();
}

QFrame* bsEditor2D::getToolFrame() {
  return m_viewer_window->getToolFrame();
}
