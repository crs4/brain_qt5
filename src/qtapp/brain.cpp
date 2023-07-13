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
#include "bsMainWindow.h"
#include "qtCommandHistory.h"
#include "qtCommandHistoryBrowser.h"

#include <string.h>
#include <stdio.h>
#include <qstatusbar.h>
#include <qgl.h>
#include <qapplication.h>

int main( int argc, char ** argv ) {
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication a( argc, argv );
    if ( !QGLFormat::hasOpenGL() ) {
	qWarning( "This system has no OpenGL support. Exiting." );
	return -1;
    }
    qtCommandHistory app_history;
    bsMainWindow* mw = new bsMainWindow(&app_history);
    QSize sh = mw->centralWidget()->sizeHint();

    char title[80];

    sprintf(title,"Brain `$Name$'  `$Date: 2001-10-16 07:57:30 +0200 (Tue, 16 Oct 2001) $'     CRS4");
#ifndef NDEBUG
    sprintf(title,"Brain (DEBUG) `$Name$'  `$Date: 2001-10-16 07:57:30 +0200 (Tue, 16 Oct 2001) $'     CRS4");
#endif

    mw->resize(sh.width(),
	       sh.height()*1.5f+3*mw->statusBar()->height());
    mw->setCaption( title );
    a.setMainWidget(mw);
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
