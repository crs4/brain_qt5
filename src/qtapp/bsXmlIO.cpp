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
#include "bsXmlIO.h"
#include "bsDocument.h"
#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsCortexRegion.h"
#include "bsSubRegion.h"
#include "bsHelpers.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "bsXmlHandler.h"
#include <QFile>
#include <QStringList>
#include <QXmlInputSource>

bsXmlIO::bsXmlIO(){
  m_messages.resize(maxErrorCode()+1);
  put_message(IO_ERR_WRITE_SECTION, "Error writing section");
  put_message(IO_ERR_LOAD_XML, "XML: ");
  put_message(IO_ERR_NO_SECTIONS, "No sections");
}


bsXmlIO::~bsXmlIO(){
}


bsSectionedBrain* bsXmlIO::load(const std::string& fname) {
  //std::cerr << "LOAD XML " << fname << std::endl;
  setCurrentFileName(fname);
  bsXmlHandler handler;
  QFile *xmlFile = new QFile(fname.c_str());
  QXmlInputSource source( xmlFile );
  QXmlSimpleReader reader;
  reader.setContentHandler( &handler );
  reader.setErrorHandler( &handler );
  bool ok = reader.parse( source );
  xmlFile->close();  
  if (!ok) {
    put_message(IO_ERR_LOAD_XML, handler.errorProtocol().toStdString());
    setLastErrorCode(IO_ERR_LOAD_XML);
  }
  return handler.getBrain();
}


void bsXmlIO::save(const std::string& fname, const bsSectionedBrain* b) {
setlocale(LC_NUMERIC, "C");
#define ABORT_ON_ERROR(_ok, _code) { \
  if (!(_ok)) {\
    setLastErrorCode(_code);\
    if (out) fclose(out);\
    return;\
  }\
}
     
  setCurrentFileName(fname);
  
  FILE *out  = fopen (fname.c_str(), "wb");  
  ABORT_ON_ERROR(out != NULL, IO_ERR_FILE_NOT_OPEN);
  ABORT_ON_ERROR(b != NULL, IO_ERR_NO_SECTIONS);

  fprintf(out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE brain>\n");
  fprintf(out,"<brain>\n");
  
  fprintf(out,"  <header>\n");
  {
    for (int i=0; i< b->regionLabelCount(); i++) {
      fprintf(out,"    <region_option label=\"%s\">\n",b->regionLabelItem(i).c_str());
      sl::color3f c = b->regionColorItem(i);
      fprintf(out,"      <color>%f %f %f</color>\n",c[0],c[1],c[2]);
	  //std::cerr << "COLOR " << c[0] << " " << c[1] << " " << c[2] << std::endl;
	  //printf("      <color>%f %f %f</color>\n",c[0],c[1],c[2]);
      fprintf(out,"    </region_option>\n");
    }
  }
  {
    for (int i=0; i< b->neuronTypeLabelCount(); i++) {
      fprintf(out,"    <neuron_option label=\"%s\">\n",b->neuronTypeLabelItem(i).c_str());
      sl::color3f c = b->neuronTypeColorItem(i);
      fprintf(out,"      <color>%f %f %f</color>\n",c[0],c[1],c[2]);
      bool flag = b->neuronTypeAligned(i);
      if (flag) {
	fprintf(out,"      <aligned>true</aligned>\n");
      } else {
	fprintf(out,"      <aligned>false</aligned>\n");
      }
      fprintf(out,"    </neuron_option>\n");
    }
  }
  fprintf(out,"  </header>\n");
  
  
  {
    for (int i=0; i < b->sectionCount(); i++) {
      const bsSection* s = b->sectionItem(i);
      fprintf(out,"  <section quota=\"%f\">\n",s->getQuota());
      fprintf(out,"    <section_header>\n");

      if (s->ghostSection()) {
	fprintf(out,"      <ghost>true</ghost>\n");
      } else {
	fprintf(out,"      <ghost>false</ghost>\n");
      }	

      fprintf(out,"      <points>\n");
      {
	for(int j=0; j < s->vertexCount(); j++) {
	  sl::point2f p = s->vertexItem(j);
	  fprintf(out,"        <coord>%f %f</coord>\n",p[0],p[1]);
	}
      }
      fprintf(out,"      </points>\n");

      {
	for(int tp=0; tp <= s->maxNeuronType(); tp++) {
	  fprintf(out,"      <neurons type=\"%d\">\n",tp);
	  for(int j=0; j < s->neuronCount(tp); j++) {
	    sl::point2f p = s->neuronItem(tp,j);
	    fprintf(out,"        <coord>%f %f</coord>\n",p[0],p[1]);
	  }
	  fprintf(out,"      </neurons>\n");
	}
      }
      fprintf(out,"    </section_header>\n");
      for (int ci=0; ci < s->cortexRegionCount(); ci++) {
	const bsCortexRegion* c = s->cortexRegionItem(ci);
	fprintf(out,"    <cortex_region>\n");
	fprintf(out,"      <external_boundary>");
	{
	  for (int p=0; p < c->externalBoundaryVertexCount(); p++) {
	    fprintf(out,"%d ",c->externalBoundaryVertexIndex(p));
	  }
	}
	fprintf(out,"</external_boundary>\n");
	if (c->hasInternalBoundary()) {
	  fprintf(out,"      <internal_boundary>");
	  fprintf(out,"%d ",c->getInternalBoundaryIndex());
	  fprintf(out,"</internal_boundary>\n");
	}
	{
	  for (int hi=0; hi<c->holeCount(); hi++) {
	    fprintf(out,"      <hole>");
	    for(int vi=0; vi<c->holeVertexCount(hi); vi++) {
	      fprintf(out,"%d ",c->holeVertexIndex(hi,vi));
	    }
	    fprintf(out,"</hole>\n");
	  }
	} //holes
	
	for (int ii=0; ii<c->subRegionCount(); ii++) {
	  const bsSubRegion* r = c->subRegionItem(ii);	
	  fprintf(out,"      <sub_region id=\"%d\">\n",r->getId());
	  fprintf(out,"        <external_contour>");
	  {
	    for (int p=0; p < r->externalContourVertexCount(); p++) {
	      fprintf(out,"%d ",r->externalContourVertexIndex(p));
	    }
	  }
	  fprintf(out,"</external_contour>\n");
	  fprintf(out,"        <internal_contour>");
	  {
	    for (int p=0; p < r->internalContourVertexCount(); p++) {
	      fprintf(out,"%d ",r->internalContourVertexIndex(p));
	    }
	  }
	  fprintf(out,"</internal_contour>\n");
	  fprintf(out,"        <border1>");
	  {
	    for (int p=0; p < r->border1VertexCount(); p++) {
	      fprintf(out,"%d ",r->border1VertexIndex(p));
	    }
	  }
	  fprintf(out,"</border1>\n");
	  fprintf(out,"        <border2>");
	  {
	    for (int p=0; p < r->border2VertexCount(); p++) {
	      fprintf(out,"%d ",r->border2VertexIndex(p));
	    }
	  }
	  fprintf(out,"</border2>\n");
	  {
	    for (int hi=0; hi<r->holeCount(); hi++) {
	      fprintf(out,"        <hole>");
	      for(int vi=0; vi<r->holeVertexCount(hi); vi++) {
		fprintf(out,"%d ",r->holeVertexIndex(hi,vi));
	      }
	      fprintf(out,"</hole>\n");
	    }
	  } //holes
	  fprintf(out,"      </sub_region>\n");
	} //subregion
	fprintf(out,"    </cortex_region>\n");
      } // cortexRegion
	fprintf(out,"  </section>\n");
    } // sectionCount
  }
  fprintf(out,"</brain>\n");
  fclose(out);
}

