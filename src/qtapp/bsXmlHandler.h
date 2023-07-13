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
#ifndef bsXmlHandler_h
#define bsXmlHandler_h

#include <qxml.h>
#include <qstringlist.h>
#include <iostream>
#include <sl/fixed_size_point.hpp>


class bsSectionedBrain;
class bsSection;

class bsXmlHandler : public QXmlDefaultHandler
{
public:
    bsXmlHandler();
    virtual ~bsXmlHandler();

    // return the list of quotes
    bsSectionedBrain* getBrain();

    // return the error protocol if parsing failed
    QString errorProtocol();

    // overloaded handler functions
    bool startDocument();
    bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );
    bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );
    bool characters( const QString& ch );

    QString errorString();

    bool fatalError( const QXmlParseException& exception );

protected:
    void clear();
    void clear_cortex_vectors();
    void clear_section_vectors();
  
  
private:

    // Tmp section
    bsSection* m_s;

    // State Vectors
    std::vector< sl::point2f >   m_points;
    std::vector< std::vector<sl::point2f> >   m_allneurons;
    std::vector< QString > m_fnames;

    float m_quota;
    bool m_ghost;

    std::vector< int > external_boundary;
    int internal_boundary;
    std::vector< std::vector< int > > hole;
    std::vector< int > id;
    std::vector< std::vector< int > > external_contour;
    std::vector< std::vector< int > > internal_contour;
    std::vector< std::vector< int > > border1;
    std::vector< std::vector< int > > border2;
    std::vector< std::vector< std::vector< int > > > hole_sub;
    
    // Status variables    
    int m_region_label_count;
    int m_neuron_label_count;
    int m_current_neuron_type;

    bsSectionedBrain* m_brain;
    QString m_errorProt;

    enum State {
	StateInit,
	StateDocument,
	StateHeader,
	StateRegionOption,
        StateRegionColor,
	StateNeuronOption,
        StateNeuronColor,
        StateNeuronAligned,
	StateInsideFname,
        StateSection,
        StateSectionHeader,
	StateGhost,
        StatePoints,
        StatePointCoord,
        StateNeurons,
        StateNeuronCoord,
        StateCortexRegion,
        StateExternalBoundary,
        StateInternalBoundary,
        StateHoleCortexRegion,
        StateSubregion,
        StateExternalContour,
	StateInternalContour,
	StateBorder1,
	StateBorder2,
	StateHoleSubregion,
	StateEnd,
    };

    State m_state;
};

#endif
