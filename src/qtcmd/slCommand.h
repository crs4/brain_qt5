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
#ifndef slCommand_h
#define slCommand_h

#include <assert.h>

/*!
  \class slCommand slCommand.h
  \brief Abstract notion of commands
  \ingroup commands  
  This class defines the notions of command
  execution, command undoing, and command 
  compression. New commands should be derived from 
  this class. By default, commands are
  assumed unreversible (i.e. no undoing/redoing,
  no command composition)
*/
class slCommand {
protected: // Vars

  slCommand* m_prev;
  slCommand* m_next;

public: // invariant

  virtual bool invariant() const;

protected: // Creation

  slCommand();

public: // Destruction

  virtual ~slCommand();

public: // Attributes

  virtual const char* name() const; 

public: // Command execution

  virtual void doit() = 0;

  virtual inline bool isUndoable() const {
    return false;
  }

  virtual void redo() {
    assert(isUndoable());
  }

  virtual inline void undo() {
    assert(isUndoable());
  }

public: // Command compression

  virtual inline bool isComposable(slCommand *) const {
    //assert(c);
    bool result = false;
    assert(!result || isUndoable());
    return result;
  }

  virtual inline void composeAndDestroy( slCommand *) {
    //assert(c);
    //assert(isComposable(c));
  }

public: // Sequence linking

  inline bool isLinked() const {
    return m_prev || m_next;
  }

  inline bool isLast() const {
    return !m_next;
  }

  inline bool isFirst() const {
    return !m_prev;
  }

  const slCommand* next() const {
    return m_next;
  }

  slCommand* next() {
    return m_next;
  }

  const slCommand* prev() const {
    return m_prev;
  }

  slCommand* prev() {
    return m_prev;
  }

  inline void unlink() {
    assert(invariant());
    if (m_prev) { m_prev->m_next = m_next; }
    if (m_next) { m_next->m_prev = m_prev; }
    m_prev = (slCommand*)0;
    m_next = (slCommand*)0;
    assert(invariant());
  }

  inline void insertAfter(slCommand* c) {
    assert(invariant());
    assert(c);
    assert(!isLinked());
    m_prev = c;
    m_next = c->m_next;
    c->m_next = this;
    if (m_next) { m_next->m_prev = this; }
    assert(invariant());
  }

  inline void insertBefore(slCommand* c) {
    assert(invariant());
    assert(c);
    assert(!isLinked());
    m_next = c;
    m_prev = c->m_prev;
    c->m_prev = this;
    if (m_prev) { m_prev->m_next = this; }
    assert(invariant());
  }

  void destroyAfterCommands();

  void destroyBeforeCommands();

}; // slCommand


/*!
  \class slUndoableCommand slCommand.h
  \brief Undoable commands
  \ingroup commands  
  New commands should be derived from this class 
  and define backup(), redo() and undo() to
  implement reversible command behavior.
*/
class slUndoableCommand: public slCommand {
protected:

  slUndoableCommand();

public:

  virtual ~slUndoableCommand();

  virtual bool isUndoable() const;

  virtual inline void doit() {
    backup();
    redo();
  }

  virtual void redo() = 0;
  virtual void undo() = 0;

protected:

  virtual void backup() = 0;

}; // slUndoableCommand


/*!
  \class slUncompressedCommandSequence slCommand.h
  \brief Sequences of commands of the same type. 
  \ingroup commands
  This class is used for quickly implementing undoing
  in a single step sequences of commands of the same
  class. New commands should be derived from this class 
  and define backup(), redo() and undo(). Other compression
  rules might be defined by redefining isComposable().
*/
class slUncompressedCommandSequence: public slUndoableCommand {
protected:
  slCommand* m_subsequence_first;
  slCommand* m_subsequence_last;

public:

  slUncompressedCommandSequence();

  virtual ~slUncompressedCommandSequence();

  virtual bool isComposable(slCommand *c) const;

  virtual void composeAndDestroy( slCommand *c );

  virtual void undo() = 0;
  virtual void redo() = 0;

}; // slUncompressedCommandSequence
  
#endif
