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
#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qframe.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qapplication.h>
#include <qkeycode.h>
#include "tstViewer2D.h"
#include "tstViewerWindow2D.h"
#include "qtCommandHistory.h"
#include "tstPanZoom.h"

tstViewerWindow2D::tstViewerWindow2D( QWidget* parent, 
				    const char* name,
				    qtCommandHistory* cmd_processor)
    : QWidget( parent, name )
{
  // Assign or create command processor

  m_cmd_processor = (cmd_processor) ? (cmd_processor) : new qtCommandHistory(-1);
 
  // Create a menu
  QPopupMenu *file_menu = new QPopupMenu( this );
  file_menu->insertItem( "Exit",  qApp, SLOT(quit()), CTRL+Key_Q );
  file_menu->insertItem( "Undo",  this, SLOT(undo()), CTRL+Key_Z );
  file_menu->insertItem( "Redo",  this, SLOT(redo()), CTRL+Key_T );
  
  // Create a menu bar
  QMenuBar *m = new QMenuBar( this );
  m->setSeparator( QMenuBar::InWindowsStyle );
  m->insertItem("&File", file_menu );
  
  // Create nice frames to put around the OpenGL widgets
  QFrame* f1 = new QFrame( this, "frame1" );
  f1->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  f1->setLineWidth( 2 );
  
  // Create an OpenGL widget
  m_viewer = new tstViewer2D( f1, "bsviewer" );
  
  // Create the three sliders; one for each rotation axis
  m_x_slider = new QSlider (-100, 100, 10, 0, QSlider::Vertical, this, "xsl" );
  connect( m_x_slider, SIGNAL(valueChanged(int)), this, SLOT(setXTranslation(int)) );
  //connect( m_viewer, SIGNAL(xTranslationChanged(int)), m_x_slider, SLOT(setValue(int)) );
  
  m_y_slider = new QSlider (-100, 100, 10, 0, QSlider::Vertical, this, "ysl" );
  m_y_slider->setTickmarks( QSlider::Left );
  connect( m_y_slider, SIGNAL(valueChanged(int)), this, SLOT(setYTranslation(int)) );
  //connect( m_viewer, SIGNAL(yTranslationChanged(int)), m_y_slider, SLOT(setValue(int)) );
  
  m_zoom_slider = new QSlider ( 1, 500, 10, 100, QSlider::Vertical, this, "zsl" );
  connect( m_zoom_slider, SIGNAL(valueChanged(int)), this, SLOT(setZoomFactor(int)) );
  //connect( m_viewer, SIGNAL(zoomFactorChanged(int)), m_zoom_slider, SLOT(setValue(int)) );
  connect( m_viewer, SIGNAL(zoomFactorChanged(int)), this, SLOT(updateEnableFlags(int)) );
  
  updateEnableFlags(100);
  
  // Put the sliders on top of each other
  QVBoxLayout* vlayout = new QVBoxLayout( 20, "vlayout");
  vlayout->addWidget( m_x_slider );
  vlayout->addWidget( m_y_slider );
  vlayout->addWidget( m_zoom_slider );
  
  // Put the GL widgets inside the frames
  QHBoxLayout* flayout1 = new QHBoxLayout( f1, 1, 1, "flayout1");
  flayout1->addWidget( m_viewer, 1 );
  
  // Top level layout, puts the sliders to the left of the frame/GL widget
  QHBoxLayout* hlayout = new QHBoxLayout( this, 20, 20, "hlayout");
  hlayout->setMenuBar( m );
  hlayout->addLayout( vlayout );
  hlayout->addWidget( f1, 1 );
}

void tstViewerWindow2D::updateEnableFlags(int factor) {
  m_x_slider->setEnabled(factor > 100);
  m_y_slider->setEnabled(factor > 100);
}
 
void tstViewerWindow2D::setZoomFactor(int factor) {
  tstPanZoom * cmd = new tstPanZoom(m_viewer, 'z', factor);
  m_cmd_processor->doit(cmd);
}

void tstViewerWindow2D::setXTranslation(int factor) {
  tstPanZoom * cmd = new tstPanZoom(m_viewer, 'x', factor);
  m_cmd_processor->doit(cmd);
}

void tstViewerWindow2D::setYTranslation(int factor) {
  tstPanZoom * cmd = new tstPanZoom(m_viewer, 'y', factor);
  m_cmd_processor->doit(cmd);
}

void tstViewerWindow2D::undo() {
  if (m_cmd_processor->doneCount() > 0) {
    m_cmd_processor->undo();
  }
}

void tstViewerWindow2D::redo() {
  if (m_cmd_processor->undoneCount() > 0) {
    m_cmd_processor->redo();
  }
}

