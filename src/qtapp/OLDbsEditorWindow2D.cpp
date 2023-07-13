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
#include "bsEditorWindow2D.h"
#include "bsSectionViewer2D.h"
#include "qtCommandHistory.h"
#include "bsSectionRotateTool.h"
#include "bsSectionTranslateTool.h"
#include "bsAssignSubRegionTool.h"
#include "bsSectionDeformTool.h"
#include "bsMeasureTool.h"
#include "bsDocument.h"

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

#include "rotate.xpm"
#include "rotate.xbm"

#include "transxy.xpm"
#include "transxy.xbm"

#include "assignId.xpm"
#include "assignId.xbm"

#include "measure.xpm"
#include "measure.xbm"

#include "deform.xpm"
#include "deform.xbm"

#include <iostream>

bsEditorWindow2D ::bsEditorWindow2D( QWidget* parent, const char* name,
				     WFlags fl, qtCommandHistory* cmd_processor)
    : bsViewerWindow2DImpl( parent, name, fl, cmd_processor) {
    m_current_tool = NULL;
    m_document=NULL;
    m_tool_frame = new QFrame( this, "Transform Frame" );
    m_tool_frame->setFrameShadow( QFrame::Raised );
    m_tool_frame->setFrameShape( QFrame::StyledPanel );
    m_tool_layout = new QHBoxLayout( m_tool_frame ); 
    m_tool_layout->setSpacing( 6 );
    m_tool_layout->setMargin( 5 );
    
    ///////////////
    QPixmap transxyIcon = QPixmap(transxyPixmap);
    QBitmap transxyMask(transxy_width,transxy_height,transxy_bits, TRUE);
    insertTool(new bsSectionTranslateTool(this, m_tool_frame, "Translate XY", &transxyIcon, &transxyMask));

    QPixmap rotateIcon(rotatePixmap);
    QBitmap rotateMask(rotate_width,rotate_height,rotate_bits, TRUE);
    insertTool(new bsSectionRotateTool(this, m_tool_frame, "Rotate XY", &rotateIcon, &rotateMask));

    QPixmap assignIdIcon(assignIdPixmap);
    QBitmap assignIdMask(assignId_width,assignId_height,assignId_bits, TRUE);
    insertTool(new bsAssignSubRegionTool(this, m_tool_frame, "Assign SubRegion Id", &assignIdIcon, &assignIdMask));

    QPixmap deformIcon(deformPixmap);
    QBitmap deformMask(deform_width,deform_height,deform_bits, TRUE);
    insertTool(new bsSectionDeformTool(this, m_tool_frame, "Deform Tool", &deformIcon, &deformMask));

    QPixmap measureIcon(measurePixmap);
    QBitmap measureMask(measure_width,measure_height,measure_bits, TRUE);
    insertTool(new bsMeasureTool(this, m_tool_frame, "Measure Tool", &measureIcon, &measureMask,C_BOTTOM_RIGHT));

    enableAllTools(false);

    //////////////
    QHBoxLayout* toolBox =  toolBoxLayout();
    toolBox->addWidget( m_tool_frame );

}

bsEditorWindow2D ::~bsEditorWindow2D() {
}

bsTool* bsEditorWindow2D ::getCurrentTool() {
    return m_current_tool;
}

void bsEditorWindow2D ::setCurrentTool(bsTool* tool) {
    if (tool != m_current_tool) {
	bsTool* old_tool = m_current_tool;
	m_current_tool = tool;
	if (old_tool) {
	    old_tool->reset();
	}	    
    }
}

void bsEditorWindow2D::insertTool(bsTool* tool) {
   assert(tool);
   m_tool_layout->addWidget( tool->getToolButton() );
   m_tools.push_back(tool);
}

    
void bsEditorWindow2D::setDocument(bsDocument* doc) {
    assert(doc);
    bsViewerWindow2DImpl::setDocument(doc);
    connect(m_document,SIGNAL(newBrain2D()),this,SLOT(resetAllTools()));
}


void bsEditorWindow2D::resetAllTools(){
    std::cerr << "RESET ALL TOOLS - # of tools: " << m_tools.size() << std::endl;
    bool b = m_document && m_document->hasBrain2D();
    for (int i=0; i<m_tools.size(); i++) {
	m_tools[i]->reset();
	m_tools[i]->enableTool(b);
    }
}

void bsEditorWindow2D::enableAllTools(bool b){
    std::cerr << "ENABLE ALL TOOLS: " << b << std::endl;
    for (int i=0; i<m_tools.size(); i++) {
	m_tools[i]->enableTool(b);
    }
}

void  bsEditorWindow2D::setViewCursor(QCursor c){
    getViewer()->setCursor(c);
}
