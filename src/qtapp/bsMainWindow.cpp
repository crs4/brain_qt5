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
#include "bsEditor2D.h"
#include "bsMiniFastEditor3D.h"
#include "bsFastEditor3D.h"
#include "bsHQIsoEditor3D.h"
#include "bsSectionedBrain.h"
#include "bsSection.h"
#include "bsDocument.h"
#include "bsChooseOptions.h"
#include "bsViewer.h"

#include <QImage>
#include <QWhatsThis>

#include "qtCommandHistory.h"


#include "qtCommandHistoryBrowser.h"
#include "qtCommandHistory.h"
//Added by qt3to4:
#include <QApplication>
#include <QGridLayout>
#include <QFrame>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QToolButton>
#include <QToolBar>
#include <QIcon>
#include <QTextEdit>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>


#define Max(a,b)  ((a > b) ? (a) : (b))
#define Min(a,b)  ((a > b) ? (b) : (a))

const char * fileOpenText = "<img source=\"fileopen\"> "
"Click this button to open a <em>new XML file</em>. <br><br>"
"You can also select the <b>Open command</b> from the File menu.";
const char * fileSaveText = "Click this button to save the file in XML format"
"You will be prompted for a file name.\n\n"
"You can also select the Save command from the File menu.\n\n";


bsMainWindow::bsMainWindow(qtCommandHistory* cmd_processor) {
  
  assert(cmd_processor);
  m_cmd_processor=cmd_processor;

  //    m_cmd_processor = (cmd_processor) ? (cmd_processor) : new qtCommandHistory(-1);

    m_saveOK=false;

    QPixmap openIcon, saveIcon, histIcon, undoIcon, redoIcon;

    m_file_tools = addToolBar(tr("File"));

    openIcon = QPixmap( ":/Icons/fileopen.xpm" );
    //QToolButton* fileOpen
    //= new QToolButton( openIcon, "Open File", "",
    //			   this, SLOT(loadXml()), m_file_tools, "open file" );


    QAction *fileOpenAct = new QAction(openIcon, "&Open");
    fileOpenAct->setWhatsThis(fileOpenText);
    fileOpenAct->setShortcut(Qt::CTRL+Qt::Key_O);
    connect(fileOpenAct, SIGNAL(triggered()), this, SLOT(loadXml()));
    QToolButton* fileOpen = new QToolButton;
    fileOpen->setDefaultAction(fileOpenAct);
    m_file_tools->addWidget(fileOpen);

    saveIcon = QPixmap( ":/Icons/filesave.xpm" );
    QAction *fileSaveAct = new QAction(saveIcon, "&Save");
    fileSaveAct->setWhatsThis(fileSaveText);
    fileOpenAct->setShortcut(Qt::CTRL+Qt::Key_S);
    connect(fileSaveAct, SIGNAL(triggered()), this, SLOT(saveXml()));
    m_file_save_xml = new QToolButton;
    m_file_save_xml->setDefaultAction(fileOpenAct);
    m_file_tools->addWidget(m_file_save_xml);

    histIcon = QPixmap( ":/Icons/filehist.xpm" );
    QAction* fileHistAct = new QAction(histIcon, "&History");
    fileHistAct->setShortcut(Qt::CTRL+Qt::Key_H);
    connect(fileHistAct, SIGNAL(triggered()), this, SLOT(showHist()));
    QToolButton* fileHist = new QToolButton;
    fileHist->setCheckable(true);
    fileHist->setDefaultAction(fileHistAct);
    m_file_tools->addWidget(fileHist);


    undoIcon = QPixmap( ":/Icons/undo.xpm" );
    QAction *undoAct = new QAction(undoIcon, "&Undo");
    undoAct->setShortcut(Qt::CTRL+Qt::Key_U);
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));
    m_undo_button = new QToolButton;
    m_undo_button->setDefaultAction(undoAct);
    m_file_tools->addWidget(m_undo_button);

    redoIcon = QPixmap( ":/Icons/redo.xpm" );
    QAction *redoAct = new QAction(redoIcon, "&Redo");
    redoAct->setShortcut(Qt::CTRL+Qt::Key_T);
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));
    m_redo_button = new QToolButton;
    m_redo_button->setDefaultAction(redoAct);
    m_file_tools->addWidget(m_redo_button);

    m_file = menuBar()->addMenu(tr("&File"));
    m_file->addAction("&Load Settings",
		      this, SLOT(loadSettings()));
    m_file->addAction("&Save Settings",
		      this, SLOT(saveSettings()));
    m_id_save_xml=m_file->actions().last();
    
    m_file->addSeparator();
    m_file->addAction("&Import",
		       this, SLOT(loadInside()), Qt::CTRL+Qt::Key_I );
    m_file->addAction(fileOpenAct);
    m_file->addAction(fileSaveAct);
    m_file->addAction("&Export",
		      this, SLOT(saveInside()), Qt::CTRL+Qt::Key_E);
    m_file->addSeparator();
    m_file->addAction("Pi&xmap",
		      this, SLOT(savePixmap()), Qt::CTRL+Qt::Key_X );
    m_id_pixmap=m_file->actions().last();
    
    m_file->addSeparator();
    //m_file->addAction("&Quit", qApp, SLOT( closeAllWindows() ), Qt::CTRL+Qt::Key_Q );
    m_file->addAction("&Quit", QApplication::instance(), SLOT(closeAllWindows()), Qt::CTRL+Qt::Key_Q );
    
    m_edit = menuBar()->addMenu(tr("&Edit"));
    m_edit->addAction(undoAct);
    m_id_undo=m_edit->actions().last();
    
    m_edit->addAction(redoAct);
    m_id_redo=m_edit->actions().last();
    
    m_edit->addAction(fileHistAct);
    m_id_hist=m_edit->actions().last();
    m_id_hist_flag=false;
    //m_edit->setCheckable(true);

    m_options = menuBar()->addMenu(tr("&Options"));
    m_id_sub = m_options->addAction("&Subregions...",
				    this, SLOT(showSubregionOptions()),
				    Qt::CTRL+Qt::Key_B );
    m_id_neuro = m_options->addAction("&Neurons...",
				      this, SLOT(showNeuronOptions()),
				      Qt::CTRL+Qt::Key_N);

    m_hb = new qtCommandHistoryBrowser();
    m_hb->setHistory(m_cmd_processor);

    connect( m_cmd_processor, SIGNAL(historyChanged(qtCommandHistory*)), this, SLOT(updateMainWindow()));
    updateMainWindow();

    //QPixmap logoIcon(crs4Pixmap);
    QMenu* help = menuBar()->addMenu(tr("&Help"));
    menuBar()->addSeparator();
    //menuBar()->addMenu(logoIcon,"CRS4");

    help->addAction( "&About", this, SLOT(about()), Qt::Key_F1 );
    help->addAction( "About &CRS4", this, SLOT(aboutCRS4()) );
    help->addAction( "About &Parma", this, SLOT(aboutParma()) );
    //help->addSeparator();
    //help->addAction( "What's &This", this, SLOT(whatsThis()), Qt::SHIFT+Qt::Key_F1 );

    
    m_tab_widget = new QTabWidget(this);

    // TAB_1
    QFrame* desk_tab = new QFrame(m_tab_widget);
    QGridLayout* tabLayout = new QGridLayout(desk_tab); 
    tabLayout->setSpacing( 6 );
    tabLayout->setMargin( 11 );

    m_text_info = new QTextEdit( desk_tab );
    //m_text_info->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, 
    //					     (QSizePolicy::SizeType)5, 
    //					     m_text_info->sizePolicy().hasHeightForWidth() ) );
    m_text_info->setMinimumSize( QSize( 300, 0 ) );
    m_text_info->setMaximumSize( QSize( 400, 32767 ) );
    m_text_info->setLineWrapMode(QTextEdit::NoWrap);
    tabLayout->addWidget( m_text_info, 0, 0 );

    m_fast_viewer3D = new bsMiniFastEditor3D(desk_tab, m_cmd_processor);
    m_fast_viewer3D->setMinimumSize( QSize( 300, 300 ) );
    m_fast_viewer3D->setMaximumSize( QSize( 400, 300 ) );
    m_fast_viewer3D->setEnableSubRegions(false);
    m_fast_viewer3D->setEnableNeuronTypes(false);
    m_fast_viewer3D->setEnableSelectedRenderer(true);
    tabLayout->addWidget( m_fast_viewer3D, 1, 0 );

    m_editor2D = new bsEditor2D(desk_tab, m_cmd_processor);
    //m_editor2D->setMinimumSize( QSize( 710, 500 ) );
    tabLayout->addWidget( m_editor2D, 0, 1, 2, 1 );
    m_tab_widget->addTab( desk_tab, tr( "Section Editor" ) );

    connect( m_editor2D, SIGNAL(currentSectionChanged(int)), m_fast_viewer3D, SLOT(setSelectedRendererIndex(int)) );

    //TAB_2
    m_editor3D = new bsFastEditor3D(m_tab_widget, m_cmd_processor);
    m_editor3D->setEnableSubRegions(false);
    m_editor3D->setEnableNeuronTypes(false);
    m_tab_widget->addTab(m_editor3D, tr("3D Viewer"));

    //TAB_3
    m_hq_iso_viewer3D = new bsHQIsoEditor3D(m_tab_widget, m_cmd_processor);
    m_hq_iso_viewer3D->setEnableSubRegions(false);
    m_hq_iso_viewer3D->setEnableNeuronTypes(false);
    m_tab_widget->addTab(m_hq_iso_viewer3D, tr("HQ Iso Viewer"));


    // General
    setCentralWidget(m_tab_widget);

    m_document = new bsDocument(this, m_cmd_processor);
    connect(m_document, SIGNAL(newBrain2D(bool)), this, SLOT(enableActions()));
    connect(m_document, SIGNAL(brain2DChanged(int)), this, SLOT(updateTextInfo()));
    connect(m_document, SIGNAL(regionOptionsChanged()), this, SLOT(updateTextInfo()));
    connect(m_document, SIGNAL(neuronTypeOptionsChanged()), this, SLOT(updateTextInfo()));

    // Maybe this is not useful!
    // connect(m_tab_widget, SIGNAL(currentChanged(QWidget *)), this, SLOT(updateCurrentViewer()));
    

    m_editor2D->setDocument(m_document);
    m_editor3D->setDocument(m_document);
    m_fast_viewer3D->setDocument(m_document);
    m_hq_iso_viewer3D->setDocument(m_document);

    updateTextInfo();

    m_choose_subregion_options = new bsChooseOptions(0, true);
    m_choose_neuron_options = new bsChooseOptions(0, true);

    m_cmd_processor->clear();

    enableActions();
    
    //m_file_tools->
    
    statusBar()->showMessage( "Ready", 2000 );
}


bsMainWindow::~bsMainWindow() {
#ifndef NDEBUG
  std::cerr << "#################### Destroying bsMainWindow" << std::endl;
#endif
}


// Private Slots

void bsMainWindow::loadInside() {

    bool load_flag=true;
    if (m_document->getBrain2D()) {
	if (m_document->brain2DModified()) {
	    switch( QMessageBox::information( this, "bs Viewer",
					      "A visualization is running...\n"
					      "Save before load?",
					      "Yes", "No", "Cancel",
					      0, 2 ) ) {
	    case 0:
		saveXml();
		if(!m_saveOK) load_flag=false;
		break;
	    case 1:
		break;
	    case 2:
		load_flag=false;
		break;
	    }	

	} else {
	    switch( QMessageBox::information( this, "bs Viewer",
					      "A visualization is running...\n"
					      "Load a new one anyway?",
					      "Yes", "No",
					      0, 1 ) ) {
	    case 0:
		break;
	    case 1:
		load_flag=false;
		break;
	    }	
	}
    }
    
    if (load_flag) {
#ifndef NDEBUG
      QString dirStart;
      //QString dirStart("/xlv0/tmp/Data");
#else
	QString dirStart;
#endif
	QString fn = QFileDialog::getOpenFileName(this, "Open File", dirStart,
						  "Inside files (*.dat);;All files (*)");
	if ( !fn.isEmpty() ) {
	  loadInside( fn );
	}
	else
	    statusBar()->showMessage( "Loading aborted", 2000 );    
    }
}


void bsMainWindow::loadInside(QString fileName ) {
  m_document->loadInside(fileName);
    if (m_document->lastError() != 0){
	QMessageBox::information( this,"ERROR", m_document->lastMessage());
    } 
    statusBar()->showMessage(m_document->lastMessage(), 6000 );
}


void bsMainWindow::loadXml() {
    bool load_flag=true;
    if (m_document->getBrain2D()) {
	if (m_document->brain2DModified()) {
	    switch( QMessageBox::information( this, "bs Viewer",
					      "A visualization is running...\n"
					      "Save before load?",
					      "Yes", "No", "Cancel",
					      0, 2 ) ) {
	    case 0:
		saveXml();
		if(!m_saveOK) load_flag=false;
		break;
	    case 1:
		break;
	    case 2:
		load_flag=false;
		break;
	    }	

	} else {
	    switch( QMessageBox::information( this, "bs Viewer",
					      "A visualization is running...\n"
					      "Load a new one anyway?",
					      "Yes", "No",
					      0, 1 ) ) {
	    case 0:
		break;
	    case 1:
		load_flag=false;
		break;
	    }	
	}
    }
    
    if (load_flag) {
#ifndef NDEBUG
      QString dirStart;
      //QString dirStart("/xlv0/tmp/Data");
#else
	QString dirStart;
#endif
	QString fn = QFileDialog::getOpenFileName(this, "Open File", dirStart,
						  "XML files (*.xml);;All files (*)");
	if ( !fn.isEmpty() ) {
	    loadXml( fn );
	}
	else
	    statusBar()->showMessage( "Loading aborted", 2000 );    
    }
}


void bsMainWindow::loadXml(QString fileName ) {
    m_document->loadXml(fileName);
    if (m_document->lastError() != 0){
	QMessageBox::information( this,"ERROR", m_document->lastMessage());
    } 
    statusBar()->showMessage(m_document->lastMessage(), 6000 );
}


void bsMainWindow::saveInside()
{
  QString fn = QFileDialog::getSaveFileName(this, "Save file", "",
					    "Inside files (*.dat);;All files (*)");
    if ( !fn.isEmpty() ) {
	m_saveOK=true;
	saveInside( fn );
    }
    else {
	statusBar()->showMessage( "Save process aborted", 2000 );
	m_saveOK=false;
    }
}

void bsMainWindow::saveInside(QString fileName ) {
    m_document->saveInside(fileName);
    if (m_document->lastError() != 0){
	QMessageBox::information( this,"ERROR", m_document->lastMessage());
	m_saveOK=false;
    } 
    statusBar()->showMessage(m_document->lastMessage(), 6000 );
}


void bsMainWindow::saveXml()
{
     QString fn = QFileDialog::getSaveFileName(this, "Save file", "",
					    "XML files (*.xml);;All files (*)");

    if ( !fn.isEmpty() ) {
	m_saveOK=true;
	saveXml( fn );
    }
    else {
	statusBar()->showMessage( "Save process aborted", 2000 );
	m_saveOK=false;
    }
}

void bsMainWindow::saveXml(QString fileName ) {
    m_document->saveXml(fileName);
    if (m_document->lastError() != 0){
	QMessageBox::information( this,"ERROR", m_document->lastMessage());
	m_saveOK=false;
    } 
    statusBar()->showMessage(m_document->lastMessage(), 6000 );
}


void bsMainWindow::about()
{
    QMessageBox::about( this, "BRAIN",
			"Interactive 3-D system for reconstruction and visualization "
                        "of primates cerebral cortex.");
}

void bsMainWindow::aboutCRS4()
{
    QMessageBox::about( this, "About CRS4",
			"CRS4 - Centro di Ricerca, Sviluppo e\n"
			"Studi Superiori in Sardegna\n"
			"Visual Computing Group\n"
			"POLARIS Ed. 1\n"
			"I-09010 Pula (CA) - Italy\n"
			"Tel: +39 070 9250.1\n"
			"http://www.crs4.it/vic");
}


void bsMainWindow::aboutParma()
{
    QMessageBox::about( this, "About Parma",
			"University of Parma\n"
			"Department of Neurosciences\n"
			"Section of Physiology\n"
			"Via Volturno, 39\n"
			"I-43100 Parma (PR) - Italy\n"
			"Tel: +39 0521 903881\n"
			"Fax: +39 0521 903900\n"
			"https://www.unipr.it/");
}

void bsMainWindow::closeEvent( QCloseEvent* ce )
{
    if ( !m_document->getBrain2D()) {
        ce->accept();
        return;
    }
    
    if (m_document->brain2DModified()) {
	switch( QMessageBox::information( this, "bs Viewer",
					  "Document has been modified.\n"
					  "Save before exit?",
					  "Yes", "No", "Cancel",
					  0, 2) ) {
	case 0:
	    saveXml();
	    if(m_saveOK){
		ce->accept();
	    } else {
		ce->ignore();
	    }
	    break;
	case 1:
	    ce->accept();
	    break;
	case 2:
	    ce->ignore();
	    break;
	}
    } else {
	switch( QMessageBox::information( this, "bs Viewer",
					  "Exit. Are you sure?",
					  "Yes", "No",
					  0, 1 ) ) {
	case 0:
	    ce->accept();
	    break;
	case 1:
	    ce->ignore();
	    break;
	}
    }
}

void  bsMainWindow::updateMainWindow(){
  m_id_undo->setEnabled(m_cmd_processor->doneCount() > 0);
  m_id_redo->setEnabled(m_cmd_processor->doneCount() > 0);
  m_undo_button->setEnabled(m_cmd_processor->doneCount() > 0);
  m_redo_button->setEnabled(m_cmd_processor->undoneCount() > 0);
}

void bsMainWindow::enableActions(){
  m_id_save_xml->setEnabled(m_document->hasBrain2D());
  m_file_save_xml->setEnabled(m_document->hasBrain2D());
  m_id_sub->setEnabled(m_document->hasBrain2D());
  m_id_neuro->setEnabled(m_document->hasBrain2D());
  updateTextInfo();
}

void bsMainWindow::showHist()
{
    m_id_hist_flag = ! m_id_hist_flag;
    m_id_hist->setEnabled(m_id_hist_flag);
    if (m_id_hist_flag) {
	 m_hb->show();
    } else {
	 m_hb->hide();
    }
}



// Command

void bsMainWindow::undo() {
  if (m_cmd_processor->doneCount() > 0) {
    m_cmd_processor->undo();
  }
}


void bsMainWindow::showNeuronOptions() {
  if (m_document->hasBrain2D()) {
    m_choose_neuron_options->clear();
    bsSectionedBrain* brain = m_document->getBrain2D();
    for (int tp=0; tp <= brain->maxNeuronType(); tp++) {
      sl::color3f c = brain->neuronTypeColorItem(tp);
      QColor qc = QColor((int) (c[0]*255.0f), (int) (c[1]*255.0f), (int) (c[2]*255.0f)); 
      m_choose_neuron_options->addItem(qc,brain->neuronTypeLabelItem(tp).c_str(),
				       brain->neuronTypeAligned(tp),
				       brain->neuronTypeEnabled(tp),
				       brain->neuronTypeDimension(tp));
    }
    int result =  m_choose_neuron_options->exec();
    if ( result == QDialog::Accepted ) {
      m_document->setNeuronTypeOptions(m_choose_neuron_options);
      m_document->setNeuronSize((float) m_choose_neuron_options->getDimension());
    }
  }
}


void bsMainWindow::showSubregionOptions() {
  if (m_document->hasBrain2D()) {
    m_choose_subregion_options->clear();
    bsSectionedBrain* brain = m_document->getBrain2D();
    for (int tp=0; tp < brain->regionLabelCount(); tp++) {
      sl::color3f c = brain->regionColorItem(tp);
      QColor qc = QColor((int) (c[0]*255.0f), (int) (c[1]*255.0f), (int) (c[2]*255.0f)); 
      m_choose_subregion_options->addItem(qc,brain->regionLabelItem(tp).c_str());
    }
    int result =  m_choose_subregion_options->exec();
    if ( result == QDialog::Accepted ) {
      m_document->setRegionOptions(m_choose_subregion_options);
      m_document->setBorderSize((float) m_choose_subregion_options->getDimension());
    }
  }
}


void bsMainWindow::redo() {
  if (m_cmd_processor->undoneCount() > 0) {
    m_cmd_processor->redo();
  }
}


void bsMainWindow::updateTextInfo() {
  QString s;
  s= tr( "<qt>" );
  s += tr( "<center><h2>BRAIN</h2></center><p>" );
  if (m_document->hasBrain2D()) {
    bsSectionedBrain* brain = m_document->getBrain2D();
    sl::aabox3f box = brain->boundingBox();
    
    float w = box[1][0]-box[0][0];
    float l = box[1][1]-box[0][1];
    float h = box[1][2]-box[0][2];

    QString tmp;

    //tmp.sprintf("Number of sections: %d<p>",brain->sectionCount());
    s += QString::asprintf("Number of sections: %d<p>",brain->sectionCount());

    tmp = tr ( "<b>Dimensions [micron]:</b><br>" );
    s += tmp;

    //tmp.sprintf("<b>w:</b> %6.0f <b>h:</b> %6.0f <b>l:</b> %6.0f<p>",w,h,l);
    s += QString::asprintf("<b>w:</b> %6.0f <b>h:</b> %6.0f <b>l:</b> %6.0f<p>",w,h,l);

    //tmp.sprintf("<b>Volume:</b> %.3e mm^3<p>",brain->brainVolume()*1.0e-9);
    s += QString::asprintf("<b>Volume:</b> %.3e mm^3<p>",brain->brainVolume()*1.0e-9);

    QString colorA( "\"#FFFFFF\"" );
    QString colorB( "\"#DDDDDD\"" );
    QString bgcolor;
    QString color;

    s += tr( "<b>Subregions:</b>" );
    s += tr( "<center><table border=\"1\"><tr>,<th>Color</th><th>Type</th><th>Volume [mm^3]</th></tr>" );
    {
      for (int tp=0; tp < brain->regionLabelCount(); tp++) {
	sl::color3f c = brain->regionColorItem(tp);
	color=QString::asprintf("\"#%02X%02X%02X\"",(int)(c[0]*255.0f),(int)(c[1]*255.0f),(int)(c[2]*255.0f));
	bgcolor = (tp%2) ? colorB : colorA;
	tmp=QString::asprintf("<tr bgcolor=%s><td bgcolor=%s> </td><td>%s</td><td>%.3e</td></tr>",
		    bgcolor.toStdString().c_str(),color.toStdString().c_str(),brain->regionLabelItem(tp).c_str(),
		    brain->subRegionVolume(tp)*1.0e-9);
	s += tmp;
      }
    }
    s += tr( "</table></center><br>" );

    s += tr( "<b>Neural tracers:</b>" );
    s += tr( "<center><table border=\"1\"><tr>,<th>Color</th><th>Type</th><th>Count</th></tr>" );
    {
      for (int tp=0; tp <= brain->maxNeuronType(); tp++) {
	float result = 0.0f;
	for (int ns=0; ns<brain->sectionCount(); ns++) {
	  if ( ! brain->sectionItem(ns)->ghostSection() ) {
	    result += brain->sectionItem(ns)->neuronCount(tp);
	  }
	}
	sl::color3f c = brain->neuronTypeColorItem(tp);
	color=QString::asprintf("\"#%02X%02X%02X\"",(int)(c[0]*255.0f),(int)(c[1]*255.0f),(int)(c[2]*255.0f));
	bgcolor = (tp%2) ? colorB : colorA;
	tmp=QString::asprintf("<tr bgcolor=%s><td bgcolor=%s> </td><td>%s</td><td align=\"right\">%6.0f</td></tr>",
		    bgcolor.toStdString().c_str(),color.toStdString().c_str(),brain->neuronTypeLabelItem(tp).c_str(),result);
	s += tmp;
      }
    }
    s += tr( "</table></center>" );
  } else {
  }
  s += tr( "</qt>" );

  m_text_info->setHtml(s);
}


void bsMainWindow::savePixmap()
{
    QString fn = QFileDialog::getSaveFileName(this, "Save Pixmap", "",
					      "BMP (*.bmp)");
    
    if ( !fn.isEmpty() ) {
	savePixmap( fn );
    }
}

QImage bsMainWindow::getPixmapLegend() {
  
  int w=200;
  int h=300;
  QPixmap img(w,h);
  if (m_document->hasBrain2D()) {
    int init_posx1=4;
    int init_posx2=120;
    int init_posy=24;
    int delta=16;
    int box_size=10;
    int space=13;
    
    bsSectionedBrain* brain = m_document->getBrain2D();
    w=260+space;
    h=(Max(brain->regionLabelCount(),brain->regionLabelCount())+5)*delta;

    img.scaled(w,h);
    img.fill(Qt::white);

    QPainter paint;
    paint.begin(&img);
    QFont font("Helvetica",12);
    paint.setFont(font);
    
    paint.setPen(Qt::black);
    paint.drawRect(0,0,img.width(),img.height());
    paint.drawText(100,init_posy,"LEGEND");
    paint.drawText(init_posx1,init_posy+delta,"Regions");
    
    for (int tp=0; tp < brain->regionLabelCount(); tp++) {
      sl::color3f c = brain->regionColorItem(tp);
      QColor col((int)(c[0]*255.0f),(int)(c[1]*255.0f),(int)(c[2]*255.0f));
      QString s = brain->regionLabelItem(tp).c_str();
      paint.setBrush(col);
      int y=init_posy+(tp+2)*delta;
      paint.drawRect(init_posx1,y-box_size,box_size,box_size);
      paint.drawText(init_posx1+box_size+space,y,s);
    }
    

    paint.drawText(init_posx2,init_posy+delta,"Neurons");
    for (int tp=0; tp < brain->neuronTypeLabelCount(); tp++) {
      sl::color3f c = brain->neuronTypeColorItem(tp);
      QColor col((int)(c[0]*255.0f),(int)(c[1]*255.0f),(int)(c[2]*255.0f));
      QString s = brain->neuronTypeLabelItem(tp).c_str();
      paint.setBrush(col);
      int y=init_posy+(tp+2)*delta;
      paint.drawRect(init_posx2,y-box_size,box_size,box_size);
      paint.drawText(init_posx2+box_size+space,y,s);
    }
    QString sect;
    int n_sect=m_editor2D->getCurrentSectionIndex();
    sect=QString::asprintf("Current section: %3d",n_sect);
    paint.drawText(init_posx1,img.height()-delta,sect);
    paint.end();
  }
  QImage im=img.toImage(); 
  return im;
}

void bsMainWindow::drawImage(QImage* bkg, int x, int y, QImage* img) {
  if(bkg->valid(x,y)) {
    int xEnd=Min(x+img->width(),bkg->width());
    int yEnd=Min(y+img->height(),bkg->height());
    for (int yy=y; yy < yEnd; yy++) {
      for (int xx=x; xx < xEnd; xx++) {
	bkg->setPixel(xx,yy,img->pixel(xx-x,yy-y));
      }
    }
  }
}


void bsMainWindow::savePixmap( QString fileName ) {
  QImage img;
  raise();
  switch (m_tab_widget->currentIndex()) {
  case 0: {
    bsViewer* v =  m_editor2D->getViewer();
    QImage img2D=v->getGLPixmap();
    int w2D=img2D.width();
    int h2D=img2D.height();
    bsViewer* v3D =  m_fast_viewer3D->getViewer();
    QImage img3D=v3D->getGLPixmap();
    int w3D=img3D.width();
    int h3D=img3D.height();
    QImage imgLegend=getPixmapLegend();
    int wLegend=imgLegend.width();
    int hLegend=imgLegend.height();
    int w = Max(wLegend,w3D);
    int h = Max(hLegend+h3D,h2D);
    img=QImage(w+w2D+2,h,QImage::Format_RGB32);
    img.fill(qRgba(255,255,255,255));
    drawImage(&img,0,h-h3D,&img3D);
    drawImage(&img,w+2,h-h2D,&img2D);
    drawImage(&img,0,0,&imgLegend);
  }
  break;
  case 1: {
    bsViewer* v =  m_editor3D->getViewer();
    img = v->getGLPixmap();
  }
  break;
  case 2: {
    bsViewer* v =  m_hq_iso_viewer3D->getViewer();
    img = v->getGLPixmap();
  }
  break;
  }

  if (!img.save(fileName)){
    QMessageBox::information( this,"ERROR", "File NOT saved!");
    statusBar()->showMessage("File NOT saved!", 6000 );    
  } else {
    statusBar()->showMessage("File saved!", 6000 );    
  }
}


void bsMainWindow::updateCurrentViewer() {
  switch (m_tab_widget->currentIndex()) {
  case 0: 
    {
      m_editor2D->updateViewer();
      m_fast_viewer3D->updateViewer();
    }
    break;
  case 1: 
    {
      m_editor3D->updateViewer();
    }
    break;
  case 2: 
    {
      m_hq_iso_viewer3D->updateViewer();
    }
    break;
  }
}


// File of Setting Parameters

void bsMainWindow::saveSettings()
{
  QString fn = QFileDialog::getSaveFileName(this, "Save Settings", "",
					    "BSP files (*.bsp);;All files (*)");
  if ( !fn.isEmpty() ) {
    m_saveOK=true;
    saveSettings( fn );
  }
  else {
    statusBar()->showMessage( "Save process aborted", 2000 );
    m_saveOK=false;
  }
}

void bsMainWindow::saveSettings( QString fileName ) {
    m_document->saveSettings(fileName);
    if (m_document->lastError() != 0){
	QMessageBox::information( this,"ERROR", m_document->lastMessage());
	m_saveOK=false;
    } 
    statusBar()->showMessage(m_document->lastMessage(), 6000 );
}


void bsMainWindow::loadSettings() {
  QString dirStart;
  QString fn = QFileDialog::getOpenFileName(this, "Open File", dirStart,
					    "BSP files (*.bsp);;All files (*)");
  if ( !fn.isEmpty() ) {
    loadSettings( fn );
  }
  else
    statusBar()->showMessage( "Loading aborted", 2000 );    
}


void bsMainWindow::loadSettings(QString fileName) {
    m_document->loadSettings(fileName);
    if (m_document->lastError() != 0){
	QMessageBox::information( this,"ERROR", m_document->lastMessage());
    } 
    statusBar()->showMessage(m_document->lastMessage(), 6000 );
}


