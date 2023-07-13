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
#ifndef BSCUTTINGBOXWIDGETIMPL_H
#define BSCUTTINGBOXWIDGETIMPL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class bsCuttingBoxWidget; }
QT_END_NAMESPACE



#include <sl/fixed_size_point.hpp>

class bsCuttingBoxWidgetImpl : public QWidget
{ 
    Q_OBJECT

public:
    bsCuttingBoxWidgetImpl( QWidget* parent = 0 );
    ~bsCuttingBoxWidgetImpl();

    void setW(const float val);
    void setL(const float val);
    void setH(const float val);

    float getW() const {
      return m_width;
    };

    float getL() const {
      return m_length;
    }

    float getH() const {
      return m_height;
    }

    void setSliders(float vi, float vj, float vk);


public slots:

 void emit_wlhChanged();
 void emit_resetClicked();
 void emit_slidersChanged(int v);
 void resetSliders();
 void enableSliders(bool b);
 float normalizeSlider(int i) const;

signals:
   void wlhChanged();
   void resetClicked();
   void slidersChanged();

protected:
  Ui::bsCuttingBoxWidget *ui;
  float m_width;
  float m_length;
  float m_height;
};

#endif // BSCUTTINGBOXWIDGETIMPL_H
