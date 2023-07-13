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
brain_qtcmd {
  win32 {
    PRE_TARGETDEPS += $${BRAIN_DIR}/src/qtcmd/$$BUILD_SUBDIR/vic_brain_qtcmd$${TARGET_SUFFIX}.lib
    LIBS += $${BRAIN_DIR}/src/qtcmd/$$BUILD_SUBDIR/vic_brain_qtcmd$${TARGET_SUFFIX}.lib
  }
  unix {
    PRE_TARGETDEPS += $${BRAIN_DIR}/src/qtcmd/$$BUILD_SUBDIR/libvic_brain_qtcmd$${TARGET_SUFFIX}.a
    LIBS += -L$${BRAIN_DIR}/src/qtcmd/$$BUILD_SUBDIR -lvic_brain_qtcmd$${TARGET_SUFFIX}
  }

  INCLUDEPATH += $${BRAIN_DIR}/src/qtcmd/
  DEPENDPATH += $${BRAIN_DIR}/src/qtcmd/
 
  QT += 
  CONFIG+= xvic_sl 

  message(Configured for brain_qtcmd)
}