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
######################################
## Default post config

include($$SHARE_DIR/vic/qmakeconfig/default/default_post.pri)

false {
      unix:QMAKE_CXXFLAGS_RELEASE -= -O 
      unix:QMAKE_CXXFLAGS_RELEASE -= -O3
      unix:QMAKE_CXXFLAGS_RELEASE += -O0 -g -Wall -ggdb
      unix:QMAKE_CFLAGS_RELEASE -= -O 
      unix:QMAKE_CFLAGS_RELEASE -= -O3
      unix:QMAKE_CFLAGS_RELEASE += -O0 -g -Wall -ggdb
      DEFINES-=NDEBUG
      message("WARNING OVERRIDING OPTIMIZATION OPTIONS")
}



##############################
## Internal packages config

include ($$BRAIN_DIR/src/src.pri)

#####
## vic packages config

INCLUDEPATH += $$INCLUDE_DIR
LIBS += -L$$LIB_DIR


##############################
## External packages config

message ("Using external library configuration")
include($$SHARE_DIR/vic/qmakeconfig/xvic/xvic.pri)


##############################
## Dependencies

DEPENDPATH *= $$INCLUDEPATH
