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
#ifndef slCommandHistory_h
#define slCommandHistory_h

class slCommand;

/*!
  \class slCommandHistory slCommandHistory.h
  \brief Abstract notion of command history
  \ingroup commands  
  This class defines the notion of command history.
*/
class slCommandHistory {
public:
  slCommandHistory(int n = -1); 
  // -1 = unlimited, otherwise max number of
  // commands

  virtual ~slCommandHistory();

public: // Command execution

  virtual void doit(slCommand* cmd, bool tryCompression = true);

  virtual void undo();

  virtual void redo();

public: // History clearing

  virtual void clear();

  virtual void clearUndone(); 

public: // Notidication

  virtual void notifyChange();

public: // History access

  virtual int doneCount();

  virtual int undoneCount();

  virtual const slCommand* next(const slCommand* cmd) const; 

  virtual const slCommand* prev(const slCommand* cmd) const; 

  virtual const slCommand* firstDoneCommand() const;
  
  virtual const slCommand* lastDoneCommand() const;

  virtual const slCommand* firstUndoneCommand() const;
  
  virtual const slCommand* lastUndoneCommand() const;

protected: // Implementation

  virtual void deleteList(slCommand* first);

  virtual void pushDone(slCommand* cmd);
  virtual void popDone();

  virtual void pushUndone(slCommand* cmd);
  virtual void popUndone();

protected: // Data

  int        m_done_count;
  slCommand* m_done_first;
  slCommand* m_done_last;

  int        m_undone_count;
  slCommand* m_undone_first;
  slCommand* m_undone_last;

  int        m_max_history_count;
}; // slCommandHistory

#endif
