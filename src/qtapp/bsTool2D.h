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
#ifndef BSTOOL2D_H
#define BSTOOL2D_H

#include "bsTool.h"
//Added by qt3to4:
#include <QPixmap>

class bsEditor2D;

class bsTool2D : public bsTool {
    
  Q_OBJECT

public:

  bsTool2D(bsEditor2D* editor,
	   QWidget* parent,
	   const char* name,
	   QPixmap* pixmap = NULL,
	   QBitmap* bitmask = NULL,
	   int cursor_type = C_CENTER);


  virtual ~bsTool2D();

  bsEditor2D* editor2D();
  const bsEditor2D* editor2D() const;


};

#endif
