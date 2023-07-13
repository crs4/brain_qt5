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
#ifndef BSMINIFASTEDITOR3D_H
#define BSMINIFASTEDITOR3D_H

#include "bsEditor3D.h"
#include "bsFastBrainViewer.h"

#include <vector>
//Added by qt3to4:

class qtCommandHistory;
class QBoxLayout; 
class QFrame;
class bsMiniViewerWindow3DImpl;
class bsDocument;

class bsMiniFastEditor3D : public bsEditor3D { 

  Q_OBJECT

public:
    
  bsMiniFastEditor3D( QWidget* parent = 0, qtCommandHistory* cmd_processor = 0);
  ~bsMiniFastEditor3D();  
  
  bsFastBrainViewer* getFastBrainViewer();

  virtual void setDocumentViewerWindow();

  void setEnableSubRegions(bool b);
  void setEnableNeuronTypes(bool b);
  void setEnableSelectedRenderer(bool b);

public slots:
 
  void setSelectedRendererIndex(int n);  

protected:

 bsMiniViewerWindow3DImpl* m_viewer_window;

};

#endif ///  BSMINIFASTEDITOR3D_H
