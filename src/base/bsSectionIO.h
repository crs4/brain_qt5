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

#ifndef _BS_SECTIONIO_H_
#define _BS_SECTIONIO_H_

#include "bsSection.h"
#include "bsIO.h"

class bsSectionIO: public bsIO<bsSection> {
public: // Creation
  
  bsSectionIO();

  virtual ~bsSectionIO();

public: // Queries -- Messages

  enum {
    IO_ERR_READ_EXTERNAL_BOUNDARY = IO_ERR_MAX+1,
    IO_ERR_READ_INTERNAL_BOUNDARY = IO_ERR_MAX+2,
    IO_ERR_READ_REFL_DATA = IO_ERR_MAX+3,
    IO_ERR_READ_CYTO_DATA = IO_ERR_MAX+4,
    IO_ERR_CREAT_SECTION = IO_ERR_MAX+5,
    IO_ERR_READ_NEURON_TYPE_COUNTER = IO_ERR_MAX+6,
    IO_ERR_READ_NEURON = IO_ERR_MAX+7,
    IO_ERR_READ_QUOTA = IO_ERR_MAX+8,
    IO_ERR_WRITE_SECTION = IO_ERR_MAX+9,
    IO_SECTION_ERROR_MAX = IO_ERR_MAX+9,
  }; 

  virtual int maxErrorCode() const {
    return IO_SECTION_ERROR_MAX;
  }

};


#endif
