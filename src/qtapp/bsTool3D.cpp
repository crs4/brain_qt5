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
#include "bsTool3D.h"
#include "bsEditor3D.h"
#include <assert.h>
//Added by qt3to4:
#include <QPixmap>


bsTool3D::bsTool3D(bsEditor3D* editor,
		   QWidget* parent,
		   const char* name,
		   QPixmap* pixmap,
		   QBitmap* bitmask,
		   int cursor_type) :
  bsTool(editor,parent,name,pixmap,bitmask, cursor_type){
}

bsTool3D::~bsTool3D() {
}

bsEditor3D* bsTool3D::editor3D(){
  assert(m_editor);
  bsEditor3D* result = dynamic_cast<bsEditor3D*>(m_editor);
  assert(result);
  return result;
}

const bsEditor3D* bsTool3D::editor3D() const{
  assert(m_editor);
  bsEditor3D* result = dynamic_cast<bsEditor3D*>(m_editor);
  assert(result);
  return result;
}
