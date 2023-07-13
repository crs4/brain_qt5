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

#include "bsXmlHandler.h"
#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsCortexRegion.h"

#include <iostream>

bsXmlHandler::bsXmlHandler() {
  m_brain=0;
  m_s=0;
  m_errorProt = "";
  m_state = StateInit;
  m_region_label_count = -1;
  m_neuron_label_count = -1;
  m_current_neuron_type = -1;
  m_ghost = false;
}


bsXmlHandler::~bsXmlHandler() {
}


bsSectionedBrain* bsXmlHandler::getBrain()
{
    return m_brain;
}


QString bsXmlHandler::errorProtocol()
{
    return m_errorProt;
}

void bsXmlHandler::clear() {
  if (m_brain) delete m_brain;
  m_brain=0;
  m_s=0;
  m_errorProt = "";
  m_state = StateInit;
  m_region_label_count = -1;
  m_neuron_label_count = -1;
  m_current_neuron_type = -1;
  m_ghost = false;
}


bool bsXmlHandler::startDocument() {
  clear();
  m_brain = new bsSectionedBrain();
  return true;
}


bool bsXmlHandler::startElement( const QString&, const QString&, const QString& qName, const QXmlAttributes& atts )
{
    // do different actions depending on the name of the tag and the
    // state you are in
  
  if ( qName == "brain" && m_state == StateInit ) {
    m_state = StateDocument;
  } else if        ( qName == "header" && m_state == StateDocument ) {
    m_state = StateHeader;
  } else if ( qName == "region_option"      && m_state == StateHeader ) {
    m_state = StateRegionOption;
    QString label = atts.value("label");
    //std::cerr << "ASSIGN LABEL " << label.toStdString() << " count " << m_region_label_count << " - " << m_brain->regionLabelCount() << std::endl;
    m_region_label_count++;
    m_brain->createRegionLabelColor(label.toStdString());
  } else if ( qName == "color" && m_state == StateRegionOption ) {
    m_state = StateRegionColor;
  } else if ( qName == "neuron_option"      && m_state == StateHeader ) {
    m_state = StateNeuronOption;
    m_neuron_label_count++;
    QString label = atts.value("label");
    m_brain->createNeuronTypeLabel(label.toStdString());
  } else if ( qName == "color" && m_state == StateNeuronOption ) {
    m_state = StateNeuronColor;
  } else if ( qName == "aligned" && m_state == StateNeuronOption ) {
    m_state = StateNeuronAligned;
  } else if ( qName == "inside_fname"      && m_state == StateHeader ) {
    m_state = StateInsideFname;
  } else if ( qName == "section"      && m_state == StateDocument ) {
    m_state = StateSection;
    m_quota = atts.value("quota").toFloat();
    m_ghost = false;
  } else if ( qName == "ghost"      && m_state == StateSectionHeader ) {
    m_state = StateGhost;
  } else if ( qName == "points"      && m_state == StateSectionHeader ) {
    m_state = StatePoints;
  } else if ( qName == "coord"      && m_state == StatePoints ) {
    m_state = StatePointCoord;
  } else if ( qName == "neurons"      && m_state == StateSectionHeader ) {
    m_state = StateNeurons;
    m_current_neuron_type = atts.value("type").toInt();
    while ( (int)m_allneurons.size() <= m_current_neuron_type ){
      std::vector<sl::point2f> empty;
      m_allneurons.push_back(empty);
    }	
  } else if ( qName == "coord"      && m_state == StateNeurons ) {
    m_state = StateNeuronCoord;
  } else if ( qName == "section_header"      && m_state == StateSection ) {
    m_state = StateSectionHeader;
  } else if ( qName == "cortex_region"      && m_state == StateSection ) {
    m_state = StateCortexRegion;
  } else if ( qName == "external_boundary"      && m_state == StateCortexRegion ) {
    m_state = StateExternalBoundary;
  } else if ( qName == "internal_boundary"      && m_state == StateCortexRegion ) {
    m_state = StateInternalBoundary;
  } else if ( qName == "hole"      && m_state == StateCortexRegion ) {
    m_state = StateHoleCortexRegion;
  } else if ( qName == "sub_region"      && m_state == StateCortexRegion ) {
    m_state = StateSubregion;
    id.push_back(atts.value("id").toInt());
    std::vector< int > empty;
    external_contour.push_back(empty);
    internal_contour.push_back(empty);
    border1.push_back(empty);
    border2.push_back(empty);
    std::vector< std::vector< int > > empty2;
    hole_sub.push_back(empty2);
  } else if ( qName == "external_contour"      && m_state == StateSubregion ) {
    m_state = StateExternalContour;
  } else if ( qName == "internal_contour"      && m_state == StateSubregion ) {
    m_state = StateInternalContour;
  } else if ( qName == "border1"      && m_state == StateSubregion ) {
    m_state = StateBorder1;
  } else if ( qName == "border2"      && m_state == StateSubregion ) {
    m_state = StateBorder2;
  } else if ( qName == "hole"      && m_state == StateSubregion ) {
    m_state = StateHoleSubregion;
    std::vector< int > empty;
    hole_sub.back().push_back(empty);
  } else {
    // error
    return false;
  }
  return true;
}


bool bsXmlHandler::endElement( const QString&, const QString&, const QString&)
{
    // "pop" the state and do some actions
    switch ( m_state ) {
	case StateHoleSubregion:
	    m_state = StateSubregion;
	    break;
        case StateBorder2:
	    m_state = StateSubregion;
	    break;
        case StateBorder1:
	    m_state = StateSubregion;
	    break;
        case StateInternalContour:
	    m_state = StateSubregion;
	    break;
        case StateExternalContour:
	    m_state = StateSubregion;
	    break;
        case StateSubregion:
	    m_state = StateCortexRegion;
	    break;
        case StateHoleCortexRegion:
	    m_state = StateCortexRegion;
	    break;
        case StateInternalBoundary:
	    m_state = StateCortexRegion;
	    break;
        case StateExternalBoundary:
	    m_state = StateCortexRegion;
	    break;
        case StateCortexRegion:
	    m_state = StateSection;
	    m_s->appendCortexRegion(external_boundary,
				    internal_boundary,
				    hole,
				    id,
				    external_contour,
				    internal_contour,
				    border1,
				    border2,
				    hole_sub);
	    clear_cortex_vectors();
	    break;
        case StateSectionHeader:
	    m_state = StateSection;
	    m_s = new bsSection(m_points,m_allneurons,m_quota);
	    break;
        case StateNeuronCoord:
	    m_state = StateNeurons;
	    break;
        case StateNeurons:
	    m_state = StateSectionHeader;
	    break;
        case StatePointCoord:
	    m_state = StatePoints;
	    break;
        case StatePoints:
	    m_state = StateSectionHeader;
	    break;
        case StateGhost:
	    m_state = StateSectionHeader;
	    break;
        case StateSection:
	    m_state = StateDocument;
	    m_s->assignNeurons();
	    m_s->triangulateSection();
	    m_s->setGhostSection(m_ghost);
	    m_brain->appendSection(m_s);
	    clear_section_vectors();
	    break;
        case StateInsideFname:
	    m_state = StateHeader;
	    break;
        case StateNeuronAligned:
	    m_state = StateNeuronOption;
	    break;
        case StateNeuronColor:
	    m_state = StateNeuronOption;
	    break;
        case StateNeuronOption:
	    m_state = StateHeader;
	    break;
        case StateRegionColor:
	    m_state = StateRegionOption;
	    break;
        case StateRegionOption:
	    m_state = StateHeader;
	    break;
        case StateHeader:
	    m_state = StateDocument;
	    break;
        case StateDocument:
	    m_state = StateEnd;
	    m_brain->assignThickness();
	    break;
	default:
	    // do nothing
	    break;
    }
    return true;
}


bool bsXmlHandler::characters( const QString& ch )
{
    // we are not interested in whitespaces
  QString ch_simplified = ch.simplified();
    if ( ch_simplified.isEmpty() )
	return true;

    switch ( m_state ) {
    case StateRegionColor: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      //std::cerr << "FIXME StateRegionColor " << ch.toStdString() << std::endl;
      bool ok = (s.count() == 3); 
      sl::color3f color;
      if (ok) {
	bool flag;
  QLocale loc(QLocale::English, QLocale::UnitedStates);
  color[0] = loc.toFloat(s[0], &flag);
	//color[0] = s[0].toFloat(&flag);
	ok = ok && flag;
  color[1] = loc.toFloat(s[1], &flag);
	//color[1]= s[1].toFloat(&flag);
	ok = ok && flag;
  color[2] = loc.toFloat(s[2], &flag);
	//color[2]= s[2].toFloat(&flag);
	ok = ok && flag;
      }
      if (ok) {
	m_brain->assignRegionColor(m_region_label_count,color);
      } else {
	return false;
      }
    }
    break;
    case StateNeuronAligned: {
      bool aligned; 
      if (ch == "true") {
	aligned = true;
      } else if (ch == "false") {
	aligned = false;
      } else {
	aligned = false;
	return false;
      }
      m_brain->setNeuronTypeAligned(m_neuron_label_count,aligned);
    }
    break;
    case StateNeuronColor: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      //std::cerr << "FIXME StateNeuronColor " << ch.toStdString() << std::endl;
      bool ok = (s.count() == 3); 
      sl::color3f color;
      if (ok) {
	bool flag;
	color[0] = s[0].toFloat(&flag);
	ok = ok && flag;
	color[1]= s[1].toFloat(&flag);
	ok = ok && flag;
	color[2]= s[2].toFloat(&flag);
	ok = ok && flag;
      }
      if (ok) {
	//std::cerr << "FIXME StateNeuronColor OK " << color << std::endl;
	m_brain->assignNeuronTypeColor(m_neuron_label_count,color);
      } else {
	return false;
      }
    }
    break;
    case StateInsideFname: {
      m_fnames.push_back(ch);
    }
    break;
    case StateGhost: {
      m_ghost = false; 
      if (ch == "true") {
	m_ghost = true;
      } else if (ch == "false") {
	m_ghost = false;
      } else {
	m_ghost = false;
	return false;
      }
    }
    break;
    case StatePointCoord: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      bool ok = (s.count() == 2); 
      sl::point2f p;
      if (ok) {
	bool flag;
	p[0] = s[0].toFloat(&flag);
	ok = ok && flag;
	p[1]= s[1].toFloat(&flag);
	ok = ok && flag;
      }
      if (ok) {
	m_points.push_back(p);
      } else {
	return false;
      }
    }
    break;
    case StateNeuronCoord: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      bool ok = (s.count() == 2); 
      sl::point2f p;
      if (ok) {
	bool flag;
	p[0] = s[0].toFloat(&flag);
	ok = ok && flag;
	p[1]= s[1].toFloat(&flag);
	ok = ok && flag;
      }
      if (ok) {
	// Controllare che ci sia il vettore!
	m_allneurons[m_current_neuron_type].push_back(p);	
      } else {
	return false;
      }
    }
    break;
    case StateExternalBoundary: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
        bool ok = true;
        for (int i=0; i<s.count() && ok; i++) {
            bool flag;
            int index = s[i].toInt(&flag);
            ok = ok && flag;
            if (ok) {
                external_boundary.push_back(index);
            }
        }
        if (!ok) {
            return false;
        }
    }
    break;
    case StateInternalBoundary: {
      bool ok = true;
      internal_boundary = ch.toInt(&ok);
      if (!ok) {
	return false;
      }
    }
    break;
    case StateHoleCortexRegion: {
      std::vector< int > h;
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      bool ok = true;
      for (int i=0; i<(int)s.count(); i++) {
	bool flag;
	h.push_back(s[i].toInt(&flag));
	ok = ok && flag;
      }
      if (ok) {
	hole.push_back(h);
      } else {
	return false;
      } 
    }
    break;
    case StateExternalContour: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      bool ok = true;
      for (int i=0; i<(int)s.count(); i++) {
	bool flag;
	external_contour.back().push_back(s[i].toInt(&flag));
	ok = ok && flag;
      }
      if (!ok) {
	return false;
      }
    }
    break;
    case StateInternalContour: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      bool ok = true;
      for (int i=0; i<(int)s.count(); i++) {
	bool flag;
	internal_contour.back().push_back(s[i].toInt(&flag));
	ok = ok && flag;
      }
      if (!ok) {
	return false;
      }
    }
    break;
    case StateBorder1: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      bool ok = true;
      for (int i=0; i<(int)s.count(); i++) {
	bool flag;
	border1.back().push_back(s[i].toInt(&flag));
	ok = ok && flag;
      }
      if (!ok) {
	return false;
      }
    }
    break;
    case StateBorder2: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      bool ok = true;
      for (int i=0; i<(int)s.count(); i++) {
	bool flag;
	border2.back().push_back(s[i].toInt(&flag));
	ok = ok && flag;
      }
      if (!ok) {
	return false;
      }
    }
    break;
    case StateHoleSubregion: {
      QStringList s = ch.split(" ", Qt::SkipEmptyParts);
      bool ok = true;
      for (int i=0; i<(int)s.count(); i++) {
	bool flag;
	hole_sub.back().back().push_back(s[i].toInt(&flag));
	ok = ok && flag;
      }
      if (!ok) {
	return false;
      }
    }
    break;
    default:
      return false;
    }

    return true;
}


QString bsXmlHandler::errorString()
{
    return "The document is not in the BRAIN file format.\n";
}


bool bsXmlHandler::fatalError( const QXmlParseException& exception )
{
  m_errorProt = QString( "Fatal parsing error: %1 in line %2, column %3\n" )
    .arg( exception.message() )
    .arg( exception.lineNumber() )
    .arg( exception.columnNumber() );
  return QXmlDefaultHandler::fatalError( exception );
}


void bsXmlHandler::clear_cortex_vectors(){
  external_contour.clear();
  internal_contour.clear();
  border1.clear();
  border2.clear();
  hole.clear();
  hole_sub.clear();
  id.clear();
  external_boundary.clear();
  internal_boundary = -1;
}


void bsXmlHandler::clear_section_vectors(){
  m_points.clear();
  m_allneurons.clear();
}
