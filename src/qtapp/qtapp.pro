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

TEMPLATE = lib

CONFIG += xvic_sl brain_base brain_qtcmd
CONFIG += uic3
QT += opengl
QT += xml

#RESOURCES     = qtapp.qrc

#FORMS=       bsViewerWindow2D_qt4.ui

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS=       bsViewerWindow2D.ui\
             bsViewerWindow3D.ui\
             bsMiniViewerWindow3D.ui\
             bsIsoViewerWindow3D.ui \
             bsHQIsoViewerWindow3D.ui \
             bsMeasureWidget.ui \
             bsCuttingBoxWidget.ui \
             bsViewer2DOptions.ui

HEADERS= \
             bsViewer.h               bsTexImage.h \
             bsViewer2D.h             bsSectionViewer2D.h \
             bsViewerWindow2DImpl.h   tiles.h \ 
             bsViewer3D.h             bsBrainViewer.h \
	     bsFastBrainViewer.h \
	     bsHQIsoBrainViewer.h     bsTexViewer.h \   
	     bsNeuronRenderer.h \
             bsHQIsoViewerWindow3DImpl.h \
             bsViewerWindow3DImpl.h \
             bsMiniViewerWindow3DImpl.h \
	     bsEditor.h               bsEditor3D.h\
             bsEditor2D.h             bsFastEditor3D.h\
             bsMiniFastEditor3D.h \
             bsHQIsoEditor3D.h \
	     bsMainWindow.h           bsDocument.h \
             bsCmdPanZoom.h           bsCmdLoadInside.h \
             bsCmdCurrentSection.h    bsCmdUpDownSection.h \
             bsTool.h                 bsTool2D.h \
             bsSectionRotateTool.h \
             bsSectionTranslateTool.h bsTransformationTool.h \
             bsCmdTransformSection.h  bsAssignSubRegionTool.h \
             bsAssignWidget.h         bsCmdAssignSubRegion.h \
             bsSectionDeformTool.h    bsCmdSectionDeform.h \
             bsSectionEditTool.h      bsCmdSectionEdit.h \
	     bsMeasureTool.h\
	     bsMeasureWidgetImpl.h \
	     bsTool3D.h               bsTransform3DTool.h \
	     bsChooseOptions.h        bsChooseOptionsItem.h \
             mc.h \
             bsXmlIO.h                bsXmlHandler.h \
             bsCmdLoadXml.h           bsCuttingBoxTool.h \
             bsCuttingBoxWidgetImpl.h \
	     bsViewer2DOptionsImpl.h

SOURCES= \
             bsViewer.cpp               bsTexImage.cpp \
             bsViewer2D.cpp             bsSectionViewer2D.cpp \
             bsViewerWindow2DImpl.cpp   tiles.cpp\
             bsViewer3D.cpp             bsBrainViewer.cpp \
	     bsFastBrainViewer.cpp\
	     bsHQIsoBrainViewer.cpp     bsTexViewer.cpp \
	     bsNeuronRenderer.cpp \
             bsHQIsoViewerWindow3DImpl.cpp \
             bsViewerWindow3DImpl.cpp \
             bsMiniViewerWindow3DImpl.cpp \
	     bsEditor.cpp               bsEditor3D.cpp\
             bsEditor2D.cpp             bsFastEditor3D.cpp\
             bsMiniFastEditor3D.cpp \
             bsHQIsoEditor3D.cpp \
	     bsMainWindow.cpp           bsDocument.cpp \
             bsCmdPanZoom.cpp           bsCmdLoadInside.cpp \
             bsCmdCurrentSection.cpp    bsCmdUpDownSection.cpp \
             bsTool.cpp                 bsTool2D.cpp \
             bsSectionRotateTool.cpp \
             bsSectionTranslateTool.cpp bsTransformationTool.cpp \
             bsCmdTransformSection.cpp  bsAssignSubRegionTool.cpp \
             bsAssignWidget.cpp         bsCmdAssignSubRegion.cpp \
             bsSectionDeformTool.cpp    bsCmdSectionDeform.cpp \
             bsSectionEditTool.cpp      bsCmdSectionEdit.cpp \
	     bsMeasureTool.cpp\
	     bsMeasureWidgetImpl.cpp \
	     bsTool3D.cpp               bsTransform3DTool.cpp \
	     bsChooseOptions.cpp        bsChooseOptionsItem.cpp \
             mc.c \
             bsXmlIO.cpp                bsXmlHandler.cpp \
             bsCmdLoadXml.cpp           bsCuttingBoxTool.cpp \
             bsCuttingBoxWidgetImpl.cpp \
	     bsViewer2DOptionsImpl.cpp


INCLUDEPATH += $$BRAIN_DIR/qtapp/Icons

TARGET=vic_brain_qtapp

#--------------------------------------------------------------------
include(../../brain_post.pri)
#--------------------------------------------------------------------

