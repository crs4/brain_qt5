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
#ifndef BSVIEWERWINDOW3DIMPL_H
#define BSVIEWERWINDOW3DIMPL_H
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class bsViewerWindow3D; }
QT_END_NAMESPACE

//Added by qt3to4:

class bsFastBrainViewer;
class bsViewer;
class qtCommandHistory;
class bsDocument;
class QBoxLayout;
class QHBoxLayout;
class QFrame;

class bsViewerWindow3DImpl : public QWidget
{ 
    Q_OBJECT

public:
    bsViewerWindow3DImpl( QWidget* parent = 0, qtCommandHistory* cmd_processor = 0 );
    ~bsViewerWindow3DImpl();
    bsViewer* getViewer();
    bsFastBrainViewer* getViewer3D();

    inline qtCommandHistory* getCmdProcessor() {
	return m_cmd_processor;
    }

    bsDocument* getDocument();

    QBoxLayout* getToolLayout();
    QFrame* getToolFrame();

    void setEnableSubRegions(bool b);
    void setEnableNeuronTypes(bool b);
    void setEnableSelectedRenderer(bool b);


public slots:

 virtual void setDocument(bsDocument* doc);
 void setSelectedRendererIndex(int n);
 void enableAllWidgets(bool b);

protected:
    Ui::bsViewerWindow3D *ui;
    qtCommandHistory* m_cmd_processor;
    bsDocument* m_document;
    QHBoxLayout* m_tool_layout;
};

#endif // BSVIEWERWINDOW3DIMPL_H
