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
#ifndef BSEDITOR3D_H
#define BSEDITOR3D_H

#include "bsEditor.h"
#include "bsFastBrainViewer.h"

#include <vector>

class qtCommandHistory;
class QBoxLayout; 
class QFrame;
class bsViewerWindow3DImpl;
class bsDocument;

class bsEditor3D : public bsEditor { 

  Q_OBJECT

public:
    
  bsEditor3D( QWidget* parent = 0, const char* name = 0,
		    WFlags fl = 0 , qtCommandHistory* cmd_processor = 0);
  ~bsEditor3D();  
  
  virtual bsViewer* getViewer();
  virtual bsFastBrainViewer* getViewer3D();

  virtual QBoxLayout* getToolLayout();
  virtual QFrame* getToolFrame();
  virtual void setDocumentViewerWindow();

  void setEnableSubRegions(bool b);
  void setEnableNeuronTypes(bool b);
  void setEnableSelectedRenderer(bool b);

public slots:
 
    virtual void setDocument(bsDocument* doc);

    void setSelectedRendererIndex(int n);  

protected:

 bsViewerWindow3DImpl* m_viewer_window;

};

#endif ///  BSEDITOR3D_H
