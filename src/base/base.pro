#+++HDR+++
#======================================================================
#   This file is part of the BRAIN software framework.
#   Copyright (C) 2009-2022 by CRS4, Pula, Italy.
#
#   For more information, visit the CRS4 Visual Computing Group
#   web pages at http://www.crs4.it/vic/
#
#   This file may be used under the terms of the GNU General Public
#   License as published by the Free Software Foundation and appearing
#   in the file LICENSE included in the packaging of this file.
#
#   CRS4 reserves all rights not expressly granted herein.
#  
#   This file is provided AS IS with NO WARRANTY OF ANY KIND, 
#   INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS 
#   FOR A PARTICULAR PURPOSE.
#
#======================================================================
#---HDR---#
#--------------------------------------------------------------------
include(../../brain_pre.pri)
#--------------------------------------------------------------------

TEMPLATE= lib

CONFIG += xvic_sl

HEADERS= \
        bsIO.h \
        bsCortexRegion.h         bsSection.h \
        bsRegion.h               bsSubRegion.h \
        bsInsideIO.h             bsSectionIO.h \
        bsGeometryHelpers.h      bsSectionedBrain.h \
        bsSectionedBrainIO.h     bsBrainInsideIO.h \
        bsSectionRenderer.h      bsHelpers.h \
        triangle.h

SOURCES= \
        bsCortexRegion.cpp       bsSection.cpp \
        bsRegion.cpp             bsSubRegion.cpp \
        bsInsideIO.cpp           bsSectionIO.cpp \
        bsGeometryHelpers.cpp    bsSectionedBrain.cpp \
        bsSectionedBrainIO.cpp   bsBrainInsideIO.cpp \
        bsSectionRenderer.cpp    bsHelpers.cpp \
        triangle.c

TARGET=vic_brain_base

#### TRILIBRARY

DEFINES *= TRILIBRARY ANSI_DECLARATORS REDUCED NO_TIMER


#--------------------------------------------------------------------
include (../../brain_post.pri)
#--------------------------------------------------------------------
