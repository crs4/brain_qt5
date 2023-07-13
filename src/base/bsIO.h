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
// .NAME bsIO

#ifndef _BS_IO_H_
#define _BS_IO_H_

#include "bsHelpers.h"
#include <vector>

#include <string>
#include <cassert>

#include <sl/utility.hpp>

template <class T>
class bsIO {
protected: // Creation
  
  bsIO() {
    m_messages.resize(maxErrorCode()+1);
    put_message(IO_OK, "OK");
    put_message(IO_ERR_FILE_NOT_FOUND, "File not found");
    put_message(IO_ERR_FILE_NOT_OPEN, "Cannot open file");
    put_message(IO_ERR_DIR_NOT_CREAT, "Cannot create directory");
    put_message(IO_ERR_FILE_NOT_CREAT, "Cannot create file");
    m_last_message = "";
    m_current_file_name = "";
    setLastErrorCode(IO_OK);
  }
  
  virtual ~bsIO() {
    m_messages.clear();
  }
  
 public: // Queries -- Messages
  
  enum {
    IO_OK = 0,
    IO_ERR_FILE_NOT_FOUND = 1,
    IO_ERR_FILE_NOT_OPEN = 2,
    IO_ERR_DIR_NOT_CREAT = 3,
    IO_ERR_FILE_NOT_CREAT = 4,
    IO_ERR_MAX = 4,
  }; 
  
  void put_message(int code,
		   const std::string& str) {
    assert(code >= 0 && code < (int)m_messages.size());
    m_messages[code] = str;
  }
  
  inline virtual int maxErrorCode() const {
    return IO_ERR_MAX;
  }

  inline int lastErrorCode() const {
    return m_last_error;
  }
  
  inline const std::string& lastMessage() const {
    return m_last_message;
  }
  
 public: // Load
  
  virtual T* load(const std::string& fname) = 0;
  
 public: // Save
  
  virtual void save(const std::string& fname, const T* s) = 0;
  
 public: // Queries -- Current File Name
  
  inline const std::string& currentFileName() const {
    return m_current_file_name;
  }
  
  inline std::string currentDirName() const {
    return sl::pathname_directory(m_current_file_name);
  }

  inline std::string currentBaseName() const {
    return sl::pathname_base(m_current_file_name);
  }

  inline std::string completePath(const std::string& fname){
    return currentDirName() + "/" + fname;
  }

protected: // Command -- Messages
  
  inline virtual void setLastErrorCode(int err) {
    assert(err >= 0 && err <= maxErrorCode());
    m_last_error=err;
    m_last_message = currentFileName()+": "+m_messages[err];
  }

  inline void setCurrentFileName(const std::string& fname) {
    m_current_file_name=fname;
    setLastErrorCode(IO_OK);
  }

protected:
  std::string m_last_message;
  int  m_last_error;
  std::string m_current_file_name;
  std::vector<std::string> m_messages;
};

#endif // _BS_IO_H_
