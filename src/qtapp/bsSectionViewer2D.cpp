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
#include "bsSectionViewer2D.h"
#include "bsSectionedBrain.h"
#include "bsSectionRenderer.h"
#include "qtCommandHistory.h"
#include "bsCmdCurrentSection.h"
#include "bsCmdUpDownSection.h"
#include "bsDocument.h"
#include "bsTexViewer.h"

#include <QFileInfo>

#include <iostream>
#include <sl/oriented_box.hpp>

bsSectionViewer2D::bsSectionViewer2D(QWidget* parent) 
    : bsViewer2D(parent) {
    m_current_section=-1;
    m_up_count_section=0;
    m_down_count_section=0;
    m_document=NULL;
    m_render=new bsSectionRenderer();    
    m_up_render=new bsSectionRenderer();
    m_up_render->setEnableSubRegions(false);
    m_up_render->setEnableNeuronTypes(false);
    sl::color3f c;

    c = .4f, .4f, .4f;
    m_up_render->setExternalColor(c);
    m_up_render->setInternalColor(c);
    
    m_down_render=new bsSectionRenderer();
    m_down_render->setEnableSubRegions(false);
    m_down_render->setEnableNeuronTypes(false);

    c = .1f, .1f, .1f;
    m_down_render->setExternalColor(c);
    m_down_render->setInternalColor(c);

    m_background_render = new bsTexViewer();
    m_background_render->setfillcolor(m_bkg_color);
    m_background_render->setTh(210);
         
    m_neuronsize = 0.0f;
    m_bordersize = 0.0f;
    m_cutting_planes_flag = false;    
    m_image_drawing_flag = false;
    m_mask_drawing_flag = false;

    connect(this,SIGNAL(currentSectionChanged(int)),this,SLOT(loadImage()));
    //connect(this,SIGNAL(currentSectionChanged(int)),this,SLOT(loadMultiImage()));
    
}


bsSectionViewer2D::~bsSectionViewer2D() {
    if(m_render) delete m_render;
    m_render=NULL;
    if(m_up_render) delete m_up_render;
    m_up_render=NULL;
    if(m_down_render) delete m_down_render;
    m_down_render=NULL;
    if(m_background_render) delete m_background_render;
    m_background_render=NULL;
}

// Slots


void bsSectionViewer2D::setCurrentSection( int n_sect ) {
    if (m_current_section != n_sect) {
	bsCmdCurrentSection* cmd = new bsCmdCurrentSection(this, n_sect);
	m_cmd_processor->doit(cmd);
    }
}

void bsSectionViewer2D::setUpCountSection(int up_sect) {
    assert(up_sect >= 0);
    if (up_sect != m_up_count_section) {
	bsCmdUpDownSection* cmd = new bsCmdUpDownSection(this, 'u',  up_sect);
	m_cmd_processor->doit(cmd);
    }
}

void bsSectionViewer2D::setDownCountSection(int down_sect) {
    assert(down_sect <= 0);
    if (down_sect != m_down_count_section) {
	bsCmdUpDownSection* cmd = new bsCmdUpDownSection(this, 'd',  down_sect);
	m_cmd_processor->doit(cmd);
    }
}


void bsSectionViewer2D::setDocument(bsDocument* doc){
    assert(doc);
    m_document=doc;
    connect(m_document,SIGNAL(newBrain2D(bool)),this,SLOT(setupRenderer2D()));
    connect(m_document,SIGNAL(brain2DChanged(int)),this,SLOT(redrawOnChange()));
    connect(m_document,SIGNAL(regionOptionsChanged()),this,SLOT(setupRegionOptions()));
    connect(m_document,SIGNAL(neuronTypeOptionsChanged()),this,SLOT(setupNeuronTypeOptions()));
}


void bsSectionViewer2D::redrawOnChange(){
  updateGL();
}


void bsSectionViewer2D::fillSection(bool flag) {
  m_render->setFillRendering(flag);
  updateGL();
}


void bsSectionViewer2D::setupRegionOptions() {
  bsSectionedBrain* b = m_document->getBrain2D();
  if (b) {
    m_render->resizeSubRegionColors(b->regionLabelCount());
    for(int i = 0; i < m_render->subRegionColorCount(); i++) {
      m_render->setSubRegionColor(i,b->regionColorItem(i));
    }
  }
}

void bsSectionViewer2D::setupNeuronTypeOptions() {
  bsSectionedBrain* b = m_document->getBrain2D();
  if (b) {
    m_render->resizeNeuronTypeColors(b->neuronTypeLabelCount());
    for(int i = 0; i < m_render->neuronTypeColorCount(); i++) {
      m_render->setNeuronTypeColor(i,b->neuronTypeColorItem(i));
      m_render->setNeuronTypeAligned(i,b->neuronTypeAligned(i));
      m_render->setNeuronTypeEnabled(i,b->neuronTypeEnabled(i));
      m_render->setNeuronTypeDimension(i,b->neuronTypeDimension(i));
    }
  }
}


void bsSectionViewer2D::setupRenderer2D() {
  bsSectionedBrain* b = m_document->getBrain2D();
  if (b && b->sectionCount() >0) {
    m_current_section = 0;
    setupRegionOptions();
    setupNeuronTypeOptions();
    m_up_render->resizeSubRegionColors(b->maxSubRegionId()+1);
    m_up_render->resizeNeuronTypeColors(b->maxNeuronType()+1);
    m_down_render->resizeSubRegionColors(b->maxSubRegionId()+1);
    m_down_render->resizeNeuronTypeColors(b->maxNeuronType()+1);
  } 

  sl::aabox3f o_box = b->boundingBox();
  sl::aabox3f v_box = sl::aabox3f(o_box.center() - 0.8f * o_box.diagonal(),
				  o_box.center() + 0.8f * o_box.diagonal());			
  setInitBox(v_box);
}

// command -- rendering

void bsSectionViewer2D::paintGL(){
#ifndef NDEBUG
  std::cerr << "Section Viewer paintGL" << std::endl;
#endif 
  if (!isVisible()) return;  
  glClearColor(m_bkg_color[0],m_bkg_color[1],m_bkg_color[2], 1.0f ); 
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable(GL_DEPTH_TEST);
  glDepthFunc( GL_LEQUAL ); // to draw multiple layer with same z
  glDisable(GL_CULL_FACE);
  glShadeModel(GL_FLAT);
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);
  glMatrixMode( GL_PROJECTION );
  glLoadMatrixf( cameraP().to_pointer() );
  glMatrixMode( GL_MODELVIEW );
  glLoadMatrixf( cameraV().to_pointer() );

  //dummy();
  
  bsSectionedBrain* brain2D = m_document->getBrain2D();
  if (brain2D && brain2D->goodSectionIndex(m_current_section)) {
    const bsSection* section = brain2D->sectionItem(m_current_section);
    
    if(m_background_render->goodImage()  &&  (m_image_drawing_flag || m_mask_drawing_flag)) 
      {	
	m_background_render->render();
      }

    glPushMatrix(); {      
      if (m_document->getNeuronSize() > 0.0f) {
	m_neuronsize = m_document->getNeuronSize();
      } else {
	m_neuronsize = 2.0f * m_init_box.diagonal()[0] / 600.0f * worldToPixel();
      }
      if (m_document->getBorderSize() > 0.0f) {
	m_bordersize = m_document->getBorderSize();
      } else {
	m_bordersize = 3.0f * m_init_box.diagonal()[0] / 600.0f * worldToPixel();
      }    
      m_render->setNeuronSize(m_neuronsize);
      m_render->setBorderSize(m_bordersize);
      m_render->render(section);
      m_render->renderNeurons(section);
      m_render->setNeuronSize(1.0f);
      m_render->setBorderSize(1.0f);                
    } glPopMatrix();

    {
	for (int i = -1; i>= m_down_count_section; i--) {
	    if (brain2D->goodSectionIndex(m_current_section+i)) {
		const bsSection* section = brain2D->sectionItem(m_current_section+i);
		glPushMatrix(); {
		    m_down_render->render(section);
		} glPopMatrix();
		
	    }
	}
    }
    {
	for (int i = 1; i<= m_up_count_section; i++) {
	    if (brain2D->goodSectionIndex(m_current_section+i)) {
		const bsSection* section = brain2D->sectionItem(m_current_section+i);
		glPushMatrix(); {
		    m_up_render->render(section);
		} glPopMatrix();
		
	    }
	}
    }
  }

 
  if (cuttingPlanesEnabled()) {
    drawCuttingBox();
  }
  drawGrid();
  paintPainters();
}


// Command Set Current Section

void bsSectionViewer2D::setCurrentSectionCmd( int n_sect ) {
    assert(m_document);
    if (m_document->getBrain2D()->goodSectionIndex(n_sect)) {
	m_current_section = n_sect;
	emit currentSectionChanged(m_current_section);	
	updateGL();
    }
}


void bsSectionViewer2D::setUpCountSectionCmd( int up_sect ) {
  m_up_count_section = up_sect;
  emit upCountSectionChanged(up_sect);
  updateGL();
}


void bsSectionViewer2D::setDownCountSectionCmd( int down_sect ) {
  m_down_count_section = down_sect;
  emit downCountSectionChanged(down_sect);
  updateGL();
}


// Cutting planes

void bsSectionViewer2D::setCuttingPlanes(bool b) {
  m_cutting_planes_flag = b;
  updateGL();
}


float bsSectionViewer2D::angle(sl::vector3f& u, sl::vector3f& v) {
  float norm = u.two_norm() * v.two_norm();
  float theta = 0.0f;
  if (norm > 1E-6) { 
    float val = u.dot(v) / norm;
    if (val<-1.0f) val = -1.0f;
    if (val> 1.0f) val =  1.0f;
    theta = acos(val);
  }
  if ((u[0] * v[1] - u[1] * v[0]) < 0) {
    // clockwise;
    theta = - theta;
  }
  return theta;
}


float bsSectionViewer2D::intersection(sl::point3f& p0, sl::point3f& p1, float z) {
  float result = -1.0f;
  if (p0[2] != p1[2]) {
    float t = (z - p0[2])/(p1[2]-p0[2]);
    if ( t >= 0.0f && t <= 1.0f ) {
      result = t;
    }
  } else if (p0[2] == z) {
    result = -2.0f;
  }
  // result = -1.0f when intersection is outside the segment
  // result = -2.0f when segment lies on the plane z
  return result;
}

void bsSectionViewer2D::sortIntersectionList(std::vector<sl::point3f>& plist) {
  assert ( plist.size() > 0 );
  sl::point3f p_medio; 
  for (int i=0 ; i < (int) plist.size(); i ++) {
    p_medio[0] += plist[i][0];
    p_medio[1] += plist[i][1];
    p_medio[2] += plist[i][2];
  }  
  p_medio[0] = p_medio[0] / plist.size();
  p_medio[1] = p_medio[1] / plist.size();
  p_medio[2] = p_medio[2] / plist.size();

  sl::vector3f v0; 
  v0 = plist[0][0] - p_medio[0],
    plist[0][1] - p_medio[1],
    plist[0][2] - p_medio[2];

  for (int n = (int)plist.size()-1; n>0; n--) {
    float amax = -1000.0f;
    int imax = -1;
    for (int i=1; i<=n; i++) {
      sl::vector3f v; 
      v = plist[i][0] - p_medio[0],
	  plist[i][1] - p_medio[1],
	  plist[i][2] - p_medio[2];

      float a = angle(v0, v);
      if (a < 0.0f) {
	a += 2.0f*3.141592f;
      }
      if ( a > amax ) {
	amax = a;
	imax = i;
      }
    }
    if (imax != -1) {
      sl::point3f tmp = plist[imax];
      plist[imax] = plist[n];
      plist[n] = tmp;
    }
  }
}


void bsSectionViewer2D::drawCuttingBox() {
  if (m_document && m_document->hasBrain2D()) {
    sl::obox3f cb = m_document->cuttingBox();
    bsSectionedBrain* brain2D = m_document->getBrain2D();

    float quota = brain2D->sectionQuota(m_current_section);

    glColor3f(1.0, 0.0, 0.0);
    std::vector<sl::point3f> plist;
    std::vector< sl::tuple2i > index;

    sl::tuple2i ii ;
    ii = 1, 5;
    index.push_back(ii);
    ii = 3, 7;
    index.push_back(ii);
    ii = 2, 6;
    index.push_back(ii);
    ii = 0, 4;
    index.push_back(ii);
    ii = 0, 1;
    index.push_back(ii);
    ii = 1, 3;
    index.push_back(ii);
    ii = 3, 2;
    index.push_back(ii);
    ii = 2, 0;
    index.push_back(ii);
    ii = 4, 5;
    index.push_back(ii);
    ii = 5, 7;
    index.push_back(ii);
    ii = 7, 6;
    index.push_back(ii);
    ii = 6, 4;
    index.push_back(ii);

    for (int i=0 ; i < (int) index.size(); i ++) {
      sl::point3f p0 = cb.corner(index[i][0]);
      sl::point3f p1 = cb.corner(index[i][1]);
      sl::point3f pt;
      float t = intersection(p0,p1,quota);
      if ( t >= 0.0f ) {
	pt[0] = p0[0] + (p1[0] - p0[0]) * t;
	pt[1] = p0[1] + (p1[1] - p0[1]) * t;
	pt[2] = quota;
	plist.push_back(pt);
      } if ( t == -2.0f ) {
	plist.push_back(p1);
	plist.push_back(p0);
      }
    }
    
    if (plist.size() > 0) {
      sortIntersectionList(plist);
      glBegin(GL_LINE_LOOP); {
	for (int i=0; i < (int) plist.size(); i++ ) {
	  glVertex3f(plist[i][0], plist[i][1], plist[i][2]);
	}
      } glEnd();
    }
  }
}

void bsSectionViewer2D::loadImage() {

  bsSectionedBrain* brain2D = m_document->getBrain2D();
  if (brain2D && brain2D->goodSectionIndex(m_current_section)) {
    m_background_render->calculateImageBox(m_init_box[1][0]);
    QFileInfo info(m_document->currentFilename());
    QString name=info.absoluteFilePath(); 
    name.append(QString::asprintf("%d",(int)brain2D->sectionQuota(m_current_section)));
    name.append(".bmp");
    m_background_render->loadTrueImage(name);
  }
}

void bsSectionViewer2D::enableImageDrawing(bool b) {
  m_image_drawing_flag = b;
  loadImage();
  std::cerr << "enableImageDrawing " << b << std::endl;
  updateGL();
}

void bsSectionViewer2D::enableMask(bool b) {
  m_background_render->setMaskEnable(b);
  loadImage();
  std::cerr << "enable Mask " << b << std::endl;
  updateGL();
}
 
void bsSectionViewer2D::setThreshold(int th) {
  std::cerr << "threshold changed "  << std::endl;  
  m_background_render->setTh(th);
  loadImage();
  updateGL();
}

void bsSectionViewer2D::loadMultiImage() {

  bsSectionedBrain* brain2D = m_document->getBrain2D();
  if (brain2D && brain2D->goodSectionIndex(m_current_section)) {
    m_background_render->calculateImageBox(m_init_box[1][0]);
    QString name;
    name="/usr/local/people/gianni/Brain/data/";
    name.append(QString::asprintf("%d",(int)brain2D->sectionQuota(m_current_section)));
    name.append(".BMP");
    m_background_render->loadBufferImage(name);
  }
}

void bsSectionViewer2D::dummy(){
  QString name;
  name = "/usr/local/people/gianni/Brain/data/15600.BMP";
  m_background_render->setTh(255);
  m_background_render->loadBufferImage(name);
  m_background_render->multirender();
  
  //std::cerr << name  << std::endl;
  
}
