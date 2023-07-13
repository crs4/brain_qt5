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

#include <vector>


class qtCommandHistory;
class QBoxLayout; 
class QFrame;
class bsDocument;
class bsViewer3D;
class bsBrainViewer;

class bsEditor3D : public bsEditor { 

  Q_OBJECT

public:
    
  bsEditor3D( QWidget* parent = 0, qtCommandHistory* cmd_processor = 0);
  ~bsEditor3D();  
  
  bsViewer* getViewer();
  bsViewer3D* getViewer3D();
  bsBrainViewer* getBrainViewer();

  QBoxLayout* getToolLayout();
  QFrame* getToolFrame();


public slots:
 
    virtual void setDocument(bsDocument* doc);


protected:

 QBoxLayout* m_tool_layout;
 QFrame* m_tool_frame;
 QBoxLayout* m_top_layout;
 bsBrainViewer* m_viewer;

};

#endif ///  BSEDITOR3D_H
