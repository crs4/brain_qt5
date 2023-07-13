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
## Default config

include($(CYG_DIR)/usr/local/share/vic/qmakeconfig/default/default_pre.pri)

######################################
## Local config
CONFIG += release

win32:CONFIG += cygwin exceptions

##############################################
# BRAIN_DIR selection

!exists($$BRAIN_DIR/brain.pro) {
  BRAIN_DIR = $$PWD
  message("Defining BRAIN_DIR as $$PWD")	
  !exists($$BRAIN_DIR/brain.pro) {
    BRAIN_DIR = .
    message("Defining BRAIN_DIR as .")	
    !exists($$BRAIN_DIR/brain.pro) {
      BRAIN_DIR = ..
      message("Defining BRAIN_DIR as ..")	
      !exists($$BRAIN_DIR/brain.pro) {
         BRAIN_DIR = ../..
         message("Defining BRAIN_DIR as ../..")
         !exists($$BRAIN_DIR/brain.pro) {
            error("Cannot find brain.pro - please set BRAIN_DIR env. var")
         }
      }
    }
  }
}


message("BRAIN_DIR set to $$BRAIN_DIR")

