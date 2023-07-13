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
#ifndef BSVIEWERWINDOW2DIMPL_H
#define BSVIEWERWINDOW2DIMPL_H
//Added by qt3to4:
#include <QFrame>
#include <QBoxLayout>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class bsViewerWindow2D; }
QT_END_NAMESPACE

class bsSectionViewer2D;
class bsViewer;
class qtCommandHistory;
class bsDocument;
class QBoxLayout;
class QFrame;
class bsViewer2DOptionsImpl;

class bsViewerWindow2DImpl : public QWidget
{ 
    Q_OBJECT

public:
  bsViewerWindow2DImpl( QWidget* parent = 0, qtCommandHistory* cmd_processor = 0);
    ~bsViewerWindow2DImpl();
    
    bsViewer* getViewer();
    bsSectionViewer2D* getViewer2D();

    inline qtCommandHistory* getCmdProcessor() {
	return m_cmd_processor;
    }

    int getCurrentSectionIndex();

    bsDocument* getDocument();
    QBoxLayout* getToolLayout();
    QFrame* getToolFrame();

public slots:
    virtual void setDocument(bsDocument* doc);
    void setNewBrain();
    void updateIntersectionInfo(int n_section);
    void updateQuotaInfo(int n_section);
    void updateGhostInfo(int n_section);
    void showOptionDialog();
    void setGhostSection(bool b);

signals:
  void  currentSectionChanged(int);

protected:
  Ui::bsViewerWindow2D *ui;
  qtCommandHistory* m_cmd_processor;
  bsDocument* m_document;
  
  QHBoxLayout* m_tool_layout;
  bsViewer2DOptionsImpl* m_option_dialog;
};

#endif // BSVIEWERWINDOW2DIMPL_H
