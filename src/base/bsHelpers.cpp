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
#include "bsHelpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>


char* bsHelpers::extractDirName(const char* fname) {
    char* s=strdup(fname);
    int slen=(int)strlen(s);
    int index=0;
    {
	for (int i=0; i<slen; i++)
	    if (s[i]=='/') index=i+1;
    }
    char* result=NULL;
    if (index<slen) {
	result=(char*)malloc((slen+1)*sizeof(char));
	{
	    for (int i=0; i<index; i++)
		result[i]=s[i];
	}
	result[index]='\0';
    }
    free(s);
    return result;
}



char* bsHelpers::extractBaseName(const char* fname) {
    char* s=strdup(fname);
    int slen=(int)strlen(s);
    int index=0;
    {
	for (int i=0; i<slen; i++)
	    if (s[i]=='/') index=i+1;
    }
    char* result=NULL;
    if (index<slen) {
	result=(char*)malloc((slen+1)*sizeof(char));
	{
	    for (int i=index; i<slen; i++)
		result[i-index]=s[i];
	}
	result[slen-index]='\0';
    }
    free(s);
    return result;
}

bool bsHelpers::createPath(const char* path){
    char s[512];
    sprintf(s,"mkdir -p %s 1> /dev/null 2>&1",path);
    return system(s)==0 && errno!=EINTR && errno!=EACCES;
}


