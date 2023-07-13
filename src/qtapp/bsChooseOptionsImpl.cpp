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
#include "bsChooseOptionsImpl.h"

/* 
 *  Constructs a bsChooseOptionsImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
bsChooseOptionsImpl::bsChooseOptionsImpl( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : m_color_chooser( parent, name, modal, fl )
{
}

/*  
 *  Destroys the object and frees any allocated resources
 */
bsChooseOptionsImpl::~bsChooseOptionsImpl()
{
    // no need to delete child widgets, Qt does it all for us
}


