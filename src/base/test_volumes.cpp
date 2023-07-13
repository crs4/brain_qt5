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
#include "bsBrainInsideIO.h"
#include "bsSectionedBrain.h"
#include "bsInsideIO.h"
#include <iostream>


int main(int argc,
	 const char *argv[]) {
  if (argc != 2 && argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <fname> <vgap>" << std::endl;
  } else {
    std::cerr << "Loading " << argv[1] << std::endl;
    
    bsBrainInsideIO inside;
    
    if (argc == 3) {
      inside.sectionLoader()->setVgap((float)atof(argv[2]));
    }

    bsSectionedBrain* b = inside.load(argv[1]);
    
    std::cerr << "Error #" << inside.lastErrorCode();
    std::cerr << "  " << inside.lastMessage() << std::endl;

    if (b) {
      char r='a';
      printf("Save file? (Y/N) ");
      while (r!='Y' && r!='N' && 
	     r!='y' && r!='n' ) {
	  scanf("%c",&r);
      }
      if (r=='Y' || r=='y') {
	  char fname[150];
	  printf("Filename: ");
	  scanf("%s",fname);
	  inside.save(fname,b);
	  std::cerr << "Write Error #" << inside.lastErrorCode();
	  std::cerr << "  " << inside.lastMessage() << std::endl;
      }
    }

    std::cerr << "Brain Volume: " << b->brainVolume() << std::endl;

    float cortex_tot = 0.;
    for (int i=0; i<3; i++) {
      std::cerr << "Cortex region " << i << " Volume: " << b->cortexRegionVolume(i) << std::endl;
      cortex_tot += b->cortexRegionVolume(i);
    }
    std::cerr << "Tot Cortex: " << cortex_tot << std::endl;

    b->computeSubRegionVolumes();

    int rvc = b->regionVolumeCount();
    int rlc = b->regionLabelCount();
    std::cerr << "rvc, rlc: " << rvc << " , " << rlc << std::endl;
    float sub_id_tot = 0.;
    for (int j=0; j<rvc; j++) {
      std::cerr << "Sub Region Id " << j << " Volume: " << b->subRegionVolume(j) << std::endl;
      sub_id_tot +=  b->subRegionVolume(j);
    }
    std::cerr << "Sub id tot: " << sub_id_tot << std::endl;


    float cort_tot = 0.;
    int n_sec = b->sectionCount();
    for (int i=0; i<n_sec; i++) {
      const bsSection *section = b->sectionItem(i);
      if (section->subRegionCount(0)==0) {
	cort_tot += section->cortexRegionVolume(0);
      }
    }
    std::cerr << "Only Cort 0 tot: " << cort_tot << std::endl;
    std::cerr << "Cort 0 - sub_id_tot:" << b->cortexRegionVolume(0) - sub_id_tot << std::endl;

  }
}

