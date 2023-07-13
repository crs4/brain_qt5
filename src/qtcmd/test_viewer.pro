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
include (../brain.pri)

CONFIG += qt

TEMPLATE= app

DESTDIR=$$BUILD_SUBDIR

SOURCES=test_viewer.cpp

INCLUDEPATH += $$BRAIN_DIR/qtcmd $$BRAIN_DIR/qtcmd/$$BUILD_SUBDIR

unix:LIBS += -L$$BRAIN_DIR/qtcmd/$$BUILD_SUBDIR -lqtcmd 

TARGET=test_viewer


