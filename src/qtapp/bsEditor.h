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
#ifndef BSEDITOR_H
#define BSEDITOR_H

#include <qwidget.h>
//Added by qt3to4:
#include <QFrame>
#include <QBoxLayout>
#include <vector>

class qtCommandHistory;
class bsTool;
class bsDocument;
class bsViewer;

class bsEditor : public QWidget { 

    Q_OBJECT

public:

     bsEditor( QWidget* parent = 0, qtCommandHistory* cmd_processor = 0);
    ~bsEditor();  

    virtual bsTool* getCurrentTool();
    virtual void setCurrentTool(bsTool* tool);

    virtual void insertTool(bsTool* tool);
    virtual void setViewCursor(QCursor c);
 
    virtual qtCommandHistory* getCmdProcessor();

    virtual bsDocument* getDocument();
    virtual void setDocument(bsDocument* doc);

    virtual bsViewer* getViewer() = 0;
    virtual QBoxLayout* getToolLayout() = 0;
    virtual QFrame* getToolFrame() = 0;
    virtual void setDocumentViewerWindow() = 0;

    void updateViewer();

protected:

    virtual void enableAllTools(bool b);

public slots:

    virtual void resetAllTools();

protected:

    bsTool* m_current_tool;
    std::vector<bsTool*> m_tools;
    qtCommandHistory* m_cmd_processor;
    bsDocument* m_document;

};

#endif ///  BSEDITOR_H
