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
#include "tstViewerWindow2D.h"
#include "tstViewer2D.h"
#include <qapplication.h>
#include <qgl.h>
#include "qtCommandHistory.h"
#include "qtCommandHistoryBrowser.h"

int main( int argc, char **argv )
{
  QApplication::setColorSpec( QApplication::CustomColor );
  QApplication a(argc,argv);			

  if ( !QGLFormat::hasOpenGL() ) {
    qWarning( "This system has no OpenGL support. Exiting." );
    return -1;
  }

  qtCommandHistory app_history;

  tstViewerWindow2D w(NULL, NULL, &app_history);
  
  w.resize( 550, 350 );
  a.setMainWidget( &w );
  w.show();

#if 1
  qtCommandHistoryBrowser hb;
  hb.setHistory(&app_history);
  hb.show();
#endif

  return a.exec();

}
