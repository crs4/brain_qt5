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
#ifndef BSEDITOR2D_H
#define BSEDITOR2D_H

#include "bsEditor.h"
#include <vector>
//Added by qt3to4:
#include <QFrame>
#include <QBoxLayout>

class qtCommandHistory;
class bsViewerWindow2DImpl;
class bsSectionViewer2D;

class bsEditor2D : public bsEditor { 

  Q_OBJECT

public:
    
  bsEditor2D( QWidget* parent = 0, qtCommandHistory* cmd_processor = 0);
  ~bsEditor2D();  
  
  virtual bsViewer* getViewer();
  virtual bsSectionViewer2D* getViewer2D();

  int getCurrentSectionIndex() const;

  virtual QBoxLayout* getToolLayout();

  virtual QFrame* getToolFrame();
  virtual void setDocumentViewerWindow();

public slots:
  virtual void setDocument(bsDocument* doc);

signals:
  void  currentSectionChanged(int);

protected:

    bsViewerWindow2DImpl* m_viewer_window;

};

#endif ///  BSEDITOR2D_H
