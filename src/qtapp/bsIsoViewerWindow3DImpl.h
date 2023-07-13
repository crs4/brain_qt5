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
#ifndef BSISOVIEWERWINDOW3DIMPL_H
#define BSISOVIEWERWINDOW3DIMPL_H
#include "bsIsoViewerWindow3D.h"

class bsIsoBrainViewer;
class bsViewer;
class qtCommandHistory;
class bsDocument;
class QBoxLayout;
class QHBoxLayout;
class QFrame;

class bsIsoViewerWindow3DImpl : public bsIsoViewerWindow3D
{ 
    Q_OBJECT

public:
    bsIsoViewerWindow3DImpl( QWidget* parent = 0, const char* name = 0, 
			  WFlags fl = 0, qtCommandHistory* cmd_processor = 0 );
    ~bsIsoViewerWindow3DImpl();
    bsViewer* getViewer();
    bsIsoBrainViewer* getViewer3D();

    inline qtCommandHistory* getCmdProcessor() {
	return m_cmd_processor;
    }

    bsDocument* getDocument();

    QBoxLayout* getToolLayout();
    QFrame* getToolFrame();

    void setEnableSubRegions(bool b);
    void setEnableNeuronTypes(bool b);

public slots:

 virtual void setDocument(bsDocument* doc);
 void enableAllWidgets(bool b);

protected:
    qtCommandHistory* m_cmd_processor;
    bsDocument* m_document;
    QHBoxLayout* m_tool_layout;
};

#endif // BSISOVIEWERWINDOW3DIMPL_H
