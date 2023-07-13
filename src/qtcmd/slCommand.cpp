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
#include "slCommand.h"
#include <typeinfo>


slCommand::slCommand() {
  m_prev = 0;
  m_next = 0;
}

slCommand::~slCommand() {
  unlink();
}

bool slCommand::invariant() const {
  return 
    (prev() == 0 || prev()->next() == this) &&
    (next() == 0 || next()->prev() == this);
}

const char* slCommand::name() const{
  return "Unnamed cmd";
}

void slCommand::destroyAfterCommands() {
  slCommand* cmd = next();
  while (cmd) {
    slCommand* nextcmd = cmd->next();
    delete cmd;
    cmd = nextcmd;
  }
  assert(isLast());
}

void slCommand::destroyBeforeCommands() {
  slCommand* cmd = prev();
  while (cmd) {
    slCommand* prevcmd = cmd->prev();
    delete cmd;
    cmd = prevcmd;
  }
  assert(isFirst());
}

////////////////////////////////////////////////////

slUndoableCommand::slUndoableCommand() {
}

slUndoableCommand::~slUndoableCommand() {
}

bool slUndoableCommand::isUndoable() const {
  return true;
}

/////////////////////////////////////////////////////

slUncompressedCommandSequence::slUncompressedCommandSequence() {
  m_subsequence_first = 0;
  m_subsequence_last = 0;
}

slUncompressedCommandSequence::~slUncompressedCommandSequence() {
  if (m_subsequence_first) {
    assert(m_subsequence_last);
    m_subsequence_first->destroyAfterCommands();
    delete m_subsequence_first;
    m_subsequence_first = 0;
    m_subsequence_last = 0;
  }
  assert(!m_subsequence_first);
  assert(!m_subsequence_last);  
}
  
bool slUncompressedCommandSequence::isComposable(slCommand *c) const {
  return c && typeid(this) == typeid(c);
}

void slUncompressedCommandSequence::composeAndDestroy( slCommand *c ) {
  assert(c);
  assert(!c->isLinked());
  assert(isComposable(c));
  if (!m_subsequence_last) {
    m_subsequence_first = c;
    m_subsequence_last = c;
  } else {
    c->insertAfter(m_subsequence_last);
  }
  assert(c->isLast());
  assert(m_subsequence_last == c);
}

