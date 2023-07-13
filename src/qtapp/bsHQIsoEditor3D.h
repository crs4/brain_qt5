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
#ifndef BSHQISOEDITOR3D_H
#define BSHQISOEDITOR3D_H

#include "bsEditor3D.h"
#include "bsHQIsoBrainViewer.h"

#include <vector>
//Added by qt3to4:

class qtCommandHistory;
class bsHQIsoViewerWindow3DImpl;
class bsDocument;
class bsCuttingBoxTool;

class bsHQIsoEditor3D : public bsEditor3D { 

  Q_OBJECT

public:
    
  bsHQIsoEditor3D( QWidget* parent = 0, qtCommandHistory* cmd_processor = 0);
  ~bsHQIsoEditor3D();  
  
  bsHQIsoBrainViewer* getIsoBrainViewer();
  virtual void setDocumentViewerWindow();

  void setEnableSubRegions(bool b);
  void setEnableNeuronTypes(bool b);

  virtual void setDocument(bsDocument* doc);

protected:

 bsHQIsoViewerWindow3DImpl* m_viewer_window;
 bsCuttingBoxTool* m_cutting_box_tool;

};

#endif ///  BSHQISOEDITOR3D_H
