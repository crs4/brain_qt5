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
#ifndef BSVIEWERWINDOW2D_H
#define BSVIEWERWINDOW2D_H

#include <qwidget.h>

class tstViewer2D;
class QSlider;
class qtCommandHistory;

class tstViewerWindow2D: public QWidget {
    Q_OBJECT
public:
    tstViewerWindow2D( QWidget* parent = 0, 
		      const char* name = 0,
		      qtCommandHistory* cmd_processor = 0);

public slots:

    void updateEnableFlags(int factor);
    void setZoomFactor(int factor);
    void setXTranslation(int xfactor);
    void setYTranslation(int yfactor);
    
    void undo();
    void redo();

private:
    tstViewer2D* m_viewer;

    QSlider* m_x_slider;
    QSlider* m_y_slider;
    QSlider* m_zoom_slider;

    qtCommandHistory* m_cmd_processor;
};


#endif // BSVIEWERWINDOW2D_H
