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
#include "qtCommandHistoryBrowser.h"
#include "qtCommandHistory.h"
#include "slCommand.h"

#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

#include <iostream>

qtCommandHistoryBrowser::qtCommandHistoryBrowser()
  : QWidget(0)  {
    QGridLayout * g = new QGridLayout( this );

    g->addWidget( new QLabel( "<b>Last 10 done commands</b>", this ), 0, 0 );
    g->addWidget( new QLabel( "<b>Last 10 undone commands</b>", this ), 0, 1 );

    m_done_list = new QListWidget( this );
    g->addWidget( m_done_list, 1, 0 );
    m_done_list->setFocusPolicy( Qt::StrongFocus );

    m_undo_button = new QPushButton( "Undo", this );
    connect( m_undo_button, SIGNAL( clicked() ), this, SLOT( undo() ) );
    g->addWidget( m_undo_button, 2, 0 );

    m_undone_list = new QListWidget( this );
    g->addWidget( m_undone_list, 1, 1 );
    m_undone_list->setFocusPolicy( Qt::StrongFocus );

    m_redo_button = new QPushButton( "Redo", this );
    connect( m_redo_button, SIGNAL( clicked() ), this, SLOT( redo() ) );
    g->addWidget( m_redo_button, 2, 1 );

    m_history = 0;
}


qtCommandHistoryBrowser::~qtCommandHistoryBrowser() {
  
}
         
void qtCommandHistoryBrowser::setHistory(qtCommandHistory* h) {
  if (m_history) {
    disconnect( m_history, SIGNAL( historyChanged(qtCommandHistory* ) ), this, SLOT( setHistory( qtCommandHistory* ) ) );
  }
  m_history = h;
  m_done_list->clear();
  m_undone_list->clear();
  if (m_history) {
    {
      int n = m_history->doneCount();
      if (n>10) n=10;
      const slCommand* cmd = m_history->lastDoneCommand();
      for (int i = 0; i< n; i++) {
	m_done_list->addItem( QString::fromLatin1( cmd->name() ) );
	cmd = cmd->prev();
      }
    }
    {
      int n = m_history->undoneCount();
      if (n>10) n=10;
      const slCommand* cmd = m_history->lastUndoneCommand();
      for (int i = 0; i< n; i++) {
	m_undone_list->addItem( QString::fromLatin1( cmd->name() ) );
	cmd = cmd->prev();
      }
    }

    m_undo_button->setEnabled(m_done_list->count() > 0);
    m_redo_button->setEnabled(m_undone_list->count() > 0);

    connect( m_history, SIGNAL( historyChanged( qtCommandHistory* ) ), this, SLOT( setHistory( qtCommandHistory* ) ) );

  }
}

void qtCommandHistoryBrowser::undo() {
  if (m_history && m_history->doneCount() > 0) {
    m_history->undo();
  }
}

void qtCommandHistoryBrowser::redo() {
  if (m_history && m_history->undoneCount() > 0) {
    m_history->redo();
  }
}


