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
#ifndef BSVIEWER2DOPTIONSIMPL_H
#define BSVIEWER2DOPTIONSIMPL_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class bsViewer2DOptions; }
QT_END_NAMESPACE

class bsViewer2DOptionsImpl : public QDialog
{ 
    Q_OBJECT

public slots:
      virtual void reject();
      float getCellStepX() const;
      float getCellStepY() const;
      virtual void show();

public:
    bsViewer2DOptionsImpl( QWidget* parent = 0 );
    ~bsViewer2DOptionsImpl();
    float m_old_cell_stepX;
    float m_old_cell_stepY;

protected:
  Ui::bsViewer2DOptions *ui;


};

#endif // BSVIEWER2DOPTIONSIMPL_H
