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
#ifndef _BSMAINWINDOW_H
#define _BSMAINWINDOW_H

#include <QMainWindow>

class QToolBar;
class QToolButton;
class QTabWidget;
class QImage;
class QAction;
class QMenu;
class QTextEdit;

class bsEditor2D;
class bsMiniFastEditor3D;
class bsFastEditor3D;
class bsHQIsoEditor3D;

class bsDocument;
class qtCommandHistory;
class qtCommandHistoryBrowser;
class bsChooseOptions;

class bsMainWindow: public QMainWindow
{
    Q_OBJECT

public:
    bsMainWindow(qtCommandHistory* cmd_processor);
    virtual ~bsMainWindow();
    
protected:
  void closeEvent( QCloseEvent* );
  QImage getPixmapLegend();
  void drawImage(QImage* bkg, int x, int y, QImage* img);

private slots:

    void loadInside();
    void loadInside(QString fileName);
    void loadXml();
    void loadXml(QString fileName );
    void loadSettings();
    void loadSettings(QString fileName );

    void saveInside();
    void saveInside(QString fileName );
    void saveXml();
    void saveXml(QString fileName );
    void saveSettings();
    void saveSettings(QString fileName );
    void savePixmap();
    void savePixmap(QString fileName );

    void about();
    void aboutCRS4();
    void aboutParma();

    void undo();
    void redo();

    void updateTextInfo();
    void updateMainWindow();
    void enableActions();
    void showHist();

    void showSubregionOptions();
    void showNeuronOptions();

    void updateCurrentViewer();

private:

    bsEditor2D* m_editor2D;
    bsFastEditor3D* m_editor3D;
    bsMiniFastEditor3D* m_fast_viewer3D;
    bsHQIsoEditor3D* m_hq_iso_viewer3D;

    bsDocument* m_document;

    qtCommandHistory* m_cmd_processor;
    qtCommandHistoryBrowser* m_hb;

    QTabWidget* m_tab_widget;
    QToolBar* m_file_tools;

    QMenu* m_file;
    QToolButton* m_file_save_xml;
    QAction* m_id_save_xml;
    QAction* m_id_pixmap;

    QMenu* m_edit;
    QToolButton* m_undo_button;
    QToolButton* m_redo_button;
    QAction* m_id_undo;
    QAction* m_id_redo;
    QAction* m_id_hist;

    QMenu* m_options;
    QAction* m_id_sub;
    QAction* m_id_neuro;

    QTextEdit* m_text_info;

    bool m_id_hist_flag;
    bool m_saveOK;

    bsChooseOptions* m_choose_subregion_options;
    bsChooseOptions* m_choose_neuron_options;


};


#endif
