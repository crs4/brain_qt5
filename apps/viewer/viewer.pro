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

TEMPLATE= app
CONFIG    += brain_qtapp
QT +=  opengl xml

RESOURCES = viewer.qrc

SOURCES= main.cpp

TARGET= vic_brain_viewer

target.path=$$BIN_DIR
desktop.path=/usr/share/applications
desktop.files=vic_brain.desktop

icons16.path=/usr/share/icons/hicolor/16x16/apps
icons16.files= Icons/16x16/vic-brain.png

icons22.path=/usr/share/icons/hicolor/22x22/apps
icons22.files= Icons/22x22/vic-brain.png

icons24.path=/usr/share/icons/hicolor/24x24/apps
icons24.files= Icons/24x24/vic-brain.png

icons32.path=/usr/share/icons/hicolor/32x32/apps
icons32.files= Icons/32x32/vic-brain.png

icons64.path=/usr/share/icons/hicolor/64x64/apps
icons64.files= Icons/64x64/vic-brain.png

icons96.path=/usr/share/icons/hicolor/96x96/apps
icons96.files= Icons/96x96/vic-brain.png

icons-update.command= gtk-update-icon-cache /usr/share/icons/hicolor

INSTALLS+= target desktop icons16 icons22 icons24 icons32 icons48 \
icons64 icons96 icons-update

#--------------------------------------------------------------------
include(../../brain_post.pri)
#--------------------------------------------------------------------
