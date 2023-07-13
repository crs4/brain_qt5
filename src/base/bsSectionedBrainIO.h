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
// .NAME bsSectionedBrainIO

#ifndef _BS_SECTIONEDBRAINIO_H_
#define _BS_SECTIONEDBRAINIO_H_

#include "bsIO.h"
#include "bsSectionIO.h"

class bsSectionedBrain;

class bsSectionedBrainIO: public bsIO<bsSectionedBrain> {
protected: // Creation

  bsSectionedBrainIO();

public: // Destruction

  virtual ~bsSectionedBrainIO();

public: // Queries -- Messages

  enum {
    IO_ERR_READ_SECTION = IO_ERR_MAX+1,
    IO_ERR_WRITE_SECTION = IO_ERR_MAX+2,
    IO_ERR_NO_SECTIONS = IO_ERR_MAX+3,
    IO_SECTIONEDBRAIN_ERROR_MAX = IO_ERR_MAX+3, 
  }; 

  virtual int maxErrorCode() const {
    return IO_SECTIONEDBRAIN_ERROR_MAX;
  }

public: // Section loader

  inline bsSectionIO* sectionLoader() {
    return m_section_io;
  }

  inline const bsSectionIO* sectionLoader() const {
    return m_section_io;
  }

public: // Load & Save

  virtual bsSectionedBrain* load(const std::string& fname); 
  
  virtual void save(const std::string& fname, const bsSectionedBrain* s);

protected: // Implementation

  virtual void setLastErrorCode(int err);

  virtual bsSectionedBrain* loadheader(); 

  virtual void saveheader(const bsSectionedBrain* s);

protected: 

  bsSectionIO* m_section_io;

};

#endif // _BS_SECTIONEDBRAINIO_H_
