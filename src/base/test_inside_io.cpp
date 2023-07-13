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
#include "bsSection.h"
#include "bsInsideIO.h"
#include <stdio.h>
#include <iostream>

int main(int argc,
	 const char *argv[]) {
  if (argc != 2 && argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <fname> <vgap>" << std::endl;
  } else {
    std::cerr << "Loading " << argv[1] << std::endl;
    
    bsInsideIO inside;
    
    if (argc == 3) {
      inside.setVgap((float)atof(argv[2]));
    }

    bsSection* sect = inside.load(argv[1]);
    
    std::cerr << "Error #" << inside.lastErrorCode();
    std::cerr << "  " << inside.lastMessage() << std::endl;
    
    if (sect) {
      std::cout << *sect << std::endl;
      char r='a';
      printf("Save file? (Y/N) ");
      while (r!='Y' && r!='N' && 
	     r!='y' && r!='n' ) {
	  scanf("%c",&r);
      }
      
      if (r=='Y' || r=='y') {
	  char fname[50];
	  printf("Filename: ");
	  scanf("%s",fname);
	  inside.save(fname,sect);
	  std::cerr << "Write Error #" << inside.lastErrorCode();
	  std::cerr << "  " << inside.lastMessage() << std::endl;
      }
      printf("\n\n");
    }
  }
}

