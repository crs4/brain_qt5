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
#include "bsIO.h"

bsIO::bsIO() {
  m_messages.resize(maxErrorCode()+1);
  put_message(IO_OK, "OK");
  put_message(IO_ERR_FILE_NOT_FOUND, "File not found");
  put_message(IO_ERR_FILE_NOT_OPEN, "Cannot open file");
  put_message(IO_ERR_DIR_NOT_CREAT, "Cannot create directory");
  put_message(IO_ERR_FILE_NOT_CREAT, "Cannot create file");
  m_last_message = NULL;
  m_current_dir_name = strdup("none");
  m_current_base_name = strdup("none");
  m_current_file_name = strdup("none");
  setLastErrorCode(IO_OK);
}

bsIO::~bsIO() {
  if (m_current_file_name) free(m_current_file_name);
  if (m_current_dir_name) free(m_current_dir_name);
  if (m_current_base_name) free(m_current_base_name);
  m_current_dir_name = NULL;
  m_current_base_name = NULL;
  m_current_file_name = NULL;
  for (int i=0; i<(int)m_messages.size(); i++) {
    free(m_messages[i]);
    m_messages[i] = NULL;
  }
  m_messages.clear();
}

char* bsIO::bsIOcompletePath(const char* fname){
  int ldir=strlen(m_current_dir_name);
  int lfile=strlen(fname);
  char* result=(char*)malloc((ldir+lfile+1)*sizeof(char));
  {
    for (int i=0; i<ldir; i++){
      result[i]=m_current_dir_name[i];
    }
  }
  {
    for (int i=0; i<lfile; i++){
      result[i+ldir]=fname[i];
    }
  }
  result[lfile+ldir]='\0';
  return result;
}

void bsIO::setLastErrorCode(int err) {
  assert(err >= 0 && err <= maxErrorCode());
  
  m_last_error=err;
  if (m_last_message) free(m_last_message);
  
  m_last_message = (char*)malloc(strlen(currentFileName()) +
				 2 +
				 strlen(m_messages[err]) +
				 4);
  sprintf(m_last_message, "%s: %s",
	  currentFileName(),
	  m_messages[err]);
}

void bsIO::setCurrentFileName(const char* fname) {
  assert(fname);
  if (m_current_file_name) free(m_current_file_name);
  if (m_current_dir_name) free(m_current_dir_name);
  if (m_current_base_name) free(m_current_base_name);
  m_current_file_name=strdup(fname);
  m_current_dir_name=bsHelpers::extractDirName(fname);
  m_current_base_name=bsHelpers::extractBaseName(fname);
  setLastErrorCode(IO_OK);
}
