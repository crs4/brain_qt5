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
// .NAME bsBrainInsideIO

#ifndef _BS_BRAININSIDEIO_H_
#define _BS_BRAININSIDEIO_H_

#include "bsSectionedBrainIO.h"

class bsInsideIO;

class bsBrainInsideIO: public bsSectionedBrainIO {
public: 

  bsBrainInsideIO();

  virtual ~bsBrainInsideIO();

public: // Section loader

  inline bsInsideIO* sectionLoader() {
    return (bsInsideIO*)m_section_io;
  }

  inline const bsInsideIO* sectionLoader() const {
    return (const bsInsideIO*)m_section_io;
  }

};

#endif
