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
#include "bsSectionIO.h"
#include <string.h>

// Creation & Destruction

bsSectionIO::bsSectionIO() {
  m_messages.resize(maxErrorCode()+1);
  put_message(IO_ERR_READ_EXTERNAL_BOUNDARY, "Error reading external boundary");
  put_message(IO_ERR_READ_INTERNAL_BOUNDARY, "Error reading internal boundary");
  put_message(IO_ERR_READ_REFL_DATA,  "Error reading reflection data");
  put_message(IO_ERR_READ_CYTO_DATA,  "Error reading cyto data");
  put_message(IO_ERR_CREAT_SECTION,  "Error creating section");
  put_message(IO_ERR_READ_NEURON_TYPE_COUNTER, "Error reading neuron type counter");
  put_message(IO_ERR_READ_NEURON,  "Error reading neuron data");
  put_message(IO_ERR_READ_QUOTA,  "Error reading quota");
  put_message(IO_ERR_WRITE_SECTION,  "Error writing section");

}

bsSectionIO::~bsSectionIO() {
}

