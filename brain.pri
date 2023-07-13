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
CONFIG = warn_on release thread incremental staticlib

!exists($$BRAIN_DIR/brain.pro) {
  BRAIN_DIR = .
  !exists($$BRAIN_DIR/brain.pro) {
    BRAIN_DIR = ..
    !exists($$BRAIN_DIR/brain.pro) {
       BRAIN_DIR = ../..
       !exists($$BRAIN_DIR/brain.pro) {
          error("Cannot find brain.pro - please set BRAIN_DIR env. var")
       }
    }
  }
}

!contains(CONFIG,debug) {
  DEFINES *= NDEBUG
}

release {
   BUILD_SUBDIR=Release
}

!release {
   BUILD_SUBDIR=Debug
} 

OBJECTS_DIR=$$BUILD_SUBDIR
MOC_DIR=$$BUILD_SUBDIR
UI_DIR=$$BUILD_SUBDIR
UI_HEADERS_DIR=$$BUILD_SUBDIR
UI_SOURCES_DIR=$$BUILD_SUBDIR

#### SL

unix:SL_DIR = /usr/local/vvr/packages/sl/
unix:SL_EXEC_DIR = $$SL_DIR/i686-pc-linux-gnu-gnucc/

unix:INCLUDEPATH*= $$SL_DIR/include $$SL_EXEC_DIR/include/
unix:LIBS*= -L$$SL_EXEC_DIR/lib

unix:QMAKE_CXXFLAGS_RELEASE -= -O2
unix:QMAKE_CXXFLAGS_RELEASE += -O3  -Winline -fomit-frame-pointer -foptimize-sibling-calls -ffast-math -fno-math-errno \
              -funsafe-math-optimizations -fno-trapping-math -funroll-loops -funroll-all-loops 

#### TRILIBRARY

DEFINES *= TRILIBRARY ANSI_DECLARATORS REDUCED NO_TIMER

