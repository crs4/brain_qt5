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
#ifndef BSEDITORWINDOW2D_H
#define BSEDITORWINDOW2D_H

#include "bsEditor.h"
#include <vector>

class qtCommandHistory;
class QHBoxLayout; 
class bsViewerWindow2DImpl;

class bsEditorWindow2D : public bsEditor { 

  Q_OBJECT

public:
    
  bsEditorWindow2D( QWidget* parent = 0, const char* name = 0,
		    WFlags fl = 0 , qtCommandHistory* cmd_processor = 0);
  ~bsEditorWindow2D();  
  
  virtual bsViewer* getViewer();
  virtual bsViewerWindow2DImpl* getViewer2D();

  int getCurrentSectionIndex() const;

public slots:
 
  virtual void setDocument(bsDocument* doc);

protected:

    bsViewerWindow2DImpl* m_viewer;

};

#endif ///  BSEDITORWINDOW2D_H
