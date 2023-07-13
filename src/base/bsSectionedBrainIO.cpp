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
#include "bsSectionedBrainIO.h"
#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsHelpers.h"
#include <iostream>

bsSectionedBrainIO::bsSectionedBrainIO() {
  m_messages.resize(maxErrorCode()+1);
  put_message(IO_ERR_READ_SECTION, "Error reading section");
  put_message(IO_ERR_WRITE_SECTION, "Error writing section");
  put_message(IO_ERR_NO_SECTIONS, "No sections");
  m_section_io = NULL;
}

bsSectionedBrainIO::~bsSectionedBrainIO() {
  if (m_section_io) delete m_section_io;
  m_section_io = NULL;
}

void bsSectionedBrainIO::setLastErrorCode(int err) {
  assert(err >= 0 && err <= maxErrorCode());

  bsIO<bsSectionedBrain>::setLastErrorCode(err);

  if (err == IO_ERR_READ_SECTION ||
      err == IO_ERR_WRITE_SECTION) {
    m_last_error=err;
    m_last_message = currentFileName()+": "+m_messages[err];
  }
}


bsSectionedBrain* bsSectionedBrainIO::load(const std::string& fname) {

#define ABORT_ON_ERR(_ok, _code) { \
    if (!(_ok)) {\
       setLastErrorCode(_code);\
       if (result) delete result;\
       return NULL;\
    }\
  }

    setCurrentFileName(fname);
    
    bsSectionedBrain *result = NULL;
    bsSectionedBrain *tmp = loadheader();
    
    if (tmp) {
	result = new bsSectionedBrain();
	{ 
	    for (int i=0; i< tmp->sectionCount(); i++) {
	      std::string fnametmp=completePath(tmp->filenameItem(i));
		bsSection*  s = m_section_io->load(fnametmp);
		if (!s) {
		    std::cerr << "WARNING: " << m_section_io->lastMessage() << std::endl;
		} else {
		    result->appendSection(tmp->filenameItem(i), s);
		}
	    }
	    
	    result->sort();
	}
	
	{ 
	    result->assignDefaultRegionLabels();
	    for (int i=0; i< tmp->regionLabelCount(); i++) {
		result->assignRegionLabel(i, tmp->regionLabelItem(i));
	    }
	}
	
	{
	    result->assignDefaultNeuronTypeLabels();
	    for (int i=0; i< tmp->neuronTypeLabelCount(); i++) {
		result->assignNeuronTypeLabel(i, tmp->neuronTypeLabelItem(i));
	    }
	}
	
	delete tmp;
	{
	    bool ok = (result->sectionCount()>0); 
	    ABORT_ON_ERR(ok, IO_ERR_NO_SECTIONS);
	}
    }
    
    return result;
}


bsSectionedBrain* bsSectionedBrainIO::loadheader() {

#define ABORT_ON_ERROR(_ok, _code) { \
    if (!(_ok)) {\
       setLastErrorCode(_code);\
       if (result) delete result;\
       if (in) fclose(in);\
       return NULL;\
    }\
  }

  bsSectionedBrain* result = NULL;

  // files
  FILE *in = fopen(currentFileName().c_str(), "r");  
  ABORT_ON_ERROR(in != NULL, IO_ERR_FILE_NOT_FOUND);

  result = new bsSectionedBrain();

  char fnametmp[512];
  while (!feof(in)) {
    if (fscanf(in,"%s",fnametmp)==1) {
      result->appendSection(fnametmp, new bsSection());
    }
  }

  fclose(in);

  // Neuron Type Labels
  
  // Regions
  
  return result;
}


void bsSectionedBrainIO::save(const std::string& fname, const bsSectionedBrain* s) {
  setCurrentFileName(fname);
  saveheader(s);
  if (lastErrorCode() == IO_OK) {
    for (int i=0; i< s->sectionCount(); i++) {
      std::string complFilename =completePath(s->filenameItem(i)); 
      m_section_io->save(complFilename,
			 s->sectionItem(i));
      if (m_section_io->lastErrorCode() != IO_OK) {
	  std::cerr << "WARNING: " << m_section_io->lastMessage() << std::endl;
	  setLastErrorCode(IO_ERR_WRITE_SECTION);
      }
    }
  }
}

void bsSectionedBrainIO::saveheader(const bsSectionedBrain* s) {
#define ABORT_ON_WRITE(_ok, _code) { \
    if (!(_ok)) {\
       setLastErrorCode(_code);\
       if(out) fclose(out);\
       return;\
    }\
  }

  FILE *out=NULL;

  bool ok=bsHelpers::createPath(currentDirName().c_str());
  ABORT_ON_WRITE(ok, IO_ERR_DIR_NOT_CREAT);
  if (s->sectionCount() >= 0) {
    out = fopen(currentFileName().c_str(), "w");  
      ABORT_ON_WRITE(out != NULL, IO_ERR_FILE_NOT_CREAT);
      for (int i=0; i<s->sectionCount()-1; i++) {
	fprintf(out, "%s\n", s->filenameItem(i).c_str());
      }
      fprintf(out, "%s", s->filenameItem(s->sectionCount()-1).c_str());
      fclose(out);
      setLastErrorCode(IO_OK);
  } else {
      setLastErrorCode(IO_ERR_NO_SECTIONS);
  }
}

  
