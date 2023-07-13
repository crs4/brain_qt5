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
#include "slCommandHistory.h"
#include "slCommand.h"
#include <assert.h>


slCommandHistory::slCommandHistory(int n) {
  assert(n == -1 || n >= 0);

  m_max_history_count = n;
  m_done_count = 0;
  m_done_first = 0;
  m_done_last = 0;
  m_undone_count = 0;
  m_undone_first = 0;
  m_undone_last = 0;
}

slCommandHistory::~slCommandHistory() {
  clear();
}

// Command execution

void slCommandHistory::doit(slCommand* cmd, bool tryCompose) {
  assert(cmd);
  assert(!cmd->isLinked());

  if (m_undone_first) {
    deleteList(m_undone_first);
    m_undone_first = 0;
    m_undone_last = 0;
    m_undone_count=0;
  }

  cmd->doit();

  if (tryCompose && 
      m_done_last &&
      m_done_last->isComposable(cmd)) {
    m_done_last->composeAndDestroy(cmd);
  } else {
    pushDone(cmd);
  }

  if (!m_done_last->isUndoable()) {
    // The last command or command combination cannot
    // be undone. Clear history, it will be
    // useless anyway!

    assert(m_done_first);
    deleteList(m_done_first);
    m_done_first = 0;
    m_done_last = 0;
    m_done_count=0;
  }

  if (m_max_history_count >= 0 && 
      m_done_count > m_max_history_count) {
    slCommand* lastcmd = m_done_last;
    popDone();
    delete lastcmd;
  }

  assert(m_max_history_count < 0 ||
	 m_done_count <= m_max_history_count);
  assert(m_undone_count == 0);

  notifyChange();
}

void slCommandHistory::undo() {
  assert(doneCount() > 0);

  slCommand* cmd = m_done_last;
  popDone();
  cmd->undo();
  pushUndone(cmd);

  notifyChange();
}

void slCommandHistory::redo() {
  assert(undoneCount() > 0);

  slCommand* cmd = m_undone_last;
  popUndone();
  cmd->redo();
  pushDone(cmd);

  notifyChange();
}

// History clearing

void slCommandHistory::clear() {
  bool changed = false;
  if (m_undone_first) {
    deleteList(m_undone_first);
    m_undone_first = 0;
    m_undone_last = 0;
    m_undone_count=0;
    changed = true;
  }
  if (m_done_first) {
    deleteList(m_done_first);
    m_done_first = 0;
    m_done_last = 0;
    m_done_count=0;
    changed = true;
  }

  if (changed) {
    notifyChange();
  }
}

void slCommandHistory::clearUndone() {
  if (m_undone_first) {
    deleteList(m_undone_first);
    m_undone_first = 0;
    m_undone_last = 0;
    m_undone_count=0;
    notifyChange();
  }
} 

// Notification 

void slCommandHistory::notifyChange() {
}

// History access

int slCommandHistory::doneCount() {
  return m_done_count;
}

int slCommandHistory::undoneCount() {
  return m_undone_count;
}

const slCommand* slCommandHistory::next(const slCommand* cmd) const {
  assert(cmd);
  return cmd->next();
}

const slCommand* slCommandHistory::prev(const slCommand* cmd) const {
  assert(cmd);
  return cmd->prev();
}

const slCommand* slCommandHistory::firstDoneCommand() const {
  return m_done_first;
}

const slCommand* slCommandHistory::lastDoneCommand() const {
  return m_done_last;
}

const slCommand* slCommandHistory::firstUndoneCommand() const {
  return m_undone_first;
}

const slCommand* slCommandHistory::lastUndoneCommand() const {
  return m_undone_last;
}

// Implementation

void slCommandHistory::deleteList(slCommand* first) {
  assert(first);
  first->destroyAfterCommands();
  delete first;
  first=0;
}

void slCommandHistory::pushDone(slCommand* cmd) {
  assert(cmd);
  assert(!cmd->isLinked());

  if (m_done_last == 0) {
    assert(m_done_first == 0);
    m_done_first = cmd;
    m_done_last = cmd;
  } else {
    assert(m_done_last != 0);
    cmd->insertAfter(m_done_last);
    m_done_last = cmd;
  }
  m_done_count++;
}

void slCommandHistory::popDone() {
  assert(m_done_count > 0);
  assert(m_done_last);

  slCommand* cmd = m_done_last;
  m_done_last = cmd->prev();
  cmd->unlink();
  m_done_count--;
  if (m_done_count == 0) {
    m_done_first = 0;
  }
  assert(m_done_count >= 0);
  assert((m_done_count == 0) == (m_done_last == 0));
  assert((m_done_count == 0) == (m_done_first == 0));
}

void slCommandHistory::pushUndone(slCommand* cmd) {
  assert(cmd);
  assert(!cmd->isLinked());
  if (m_undone_last == 0) {
    assert(m_undone_first == 0);
    m_undone_first = cmd;
    m_undone_last = cmd;
  } else {
    assert(m_undone_last != 0);
    cmd->insertAfter(m_undone_last);
    m_undone_last = cmd;
  }
  m_undone_count++;
}
  
void slCommandHistory::popUndone() {
  assert(m_undone_count > 0);
  assert(m_undone_last);

  slCommand* cmd = m_undone_last;
  m_undone_last = cmd->prev();
  cmd->unlink();
  m_undone_count--;
  if (m_undone_count == 0) {
    m_undone_first = 0;
  }
  assert(m_undone_count >= 0);
  assert((m_undone_count == 0) == (m_undone_last == 0));
  assert((m_undone_count == 0) == (m_undone_first == 0));
}



