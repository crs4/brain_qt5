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
#ifndef bsXmlIO_h
#define bsXmlIO_h
#include "bsIO.h"


class bsSectionedBrain;

class bsXmlIO : public bsIO<bsSectionedBrain> {
  
public:
  
  bsXmlIO();

  ~bsXmlIO();

  enum {
    IO_ERR_WRITE_SECTION = IO_ERR_MAX+2,
    IO_ERR_NO_SECTIONS = IO_ERR_MAX+3,
    IO_ERR_LOAD_XML = IO_ERR_MAX+4,
    IO_SAVEXML_ERROR_MAX = IO_ERR_MAX+4, 
  }; 
  
  virtual int maxErrorCode() const {
    return IO_SAVEXML_ERROR_MAX;
  }


  virtual bsSectionedBrain* load(const std::string& fname); 
  virtual void save(const std::string& fname, const bsSectionedBrain* b);

}; // bsXmlIO 

#endif
