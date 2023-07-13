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
#ifndef bsCmdLoadXml_h
#define bsCmdLoadXml_h

#include "slCommand.h"

class bsDocument;

class bsCmdLoadXml: public slCommand {
  
public:
  
  bsCmdLoadXml(bsDocument* document, const char* filename);
  
  ~bsCmdLoadXml();
  
  virtual void doit();

  const char* name() const;

  
protected:
  
  char* m_filename;
  bsDocument* m_document;
  
}; // bsCmdLoadXml 

#endif
