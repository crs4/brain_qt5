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
#ifndef BSTRANSFORMATIONTOOL_H
#define BSTRANSFORMATIONTOOL_H

#include "bsTool2D.h"
#include <sl/fixed_size_point.hpp>
#include <sl/affine_map.hpp>
#include <sl/projective_map.hpp>
//Added by qt3to4:
#include <QPixmap>

class bsEditor2D;

class bsTransformationTool: public bsTool2D {
    Q_OBJECT

public:

    bsTransformationTool(bsEditor2D* editor,
			 QWidget* parent,
			 const char* name,
			 QPixmap* pixmap = NULL,
			 QBitmap* bitmask = NULL);

    virtual ~bsTransformationTool();

    void transformSection(const char t_type, const sl::affine_map2f& t);

    void transformSectionCmd(const sl::affine_map2f& t);
    
};

#endif
