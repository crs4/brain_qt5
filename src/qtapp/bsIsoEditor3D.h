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
#ifndef BSISOEDITOR3D_H
#define BSISOEDITOR3D_H

#include "bsEditor3D.h"
#include "bsIsoBrainViewer.h"

#include <vector>

class qtCommandHistory;
class QBoxLayout; 
class QFrame;
class bsIsoViewerWindow3DImpl;
class bsDocument;

class bsIsoEditor3D : public bsEditor3D { 

  Q_OBJECT

public:
    
  bsIsoEditor3D( QWidget* parent = 0, const char* name = 0,
		    WFlags fl = 0 , qtCommandHistory* cmd_processor = 0);
  ~bsIsoEditor3D();  
  
  bsIsoBrainViewer* getIsoBrainViewer();
  virtual void setDocumentViewerWindow();

  void setEnableSubRegions(bool b);
  void setEnableNeuronTypes(bool b);


protected:

 bsIsoViewerWindow3DImpl* m_viewer_window;

};

#endif ///  BSISOEDITOR3D_H
