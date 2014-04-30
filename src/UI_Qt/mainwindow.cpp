#include <QMdiSubWindow>
#include <QMessageBox>
#include <QDebug> // TODO

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addsynthwidget.h"
//#include "treedialog.h"
#include "treewidget.h"
#include "vkwidget.h"
#include "ui_qt_config.h"

void MainWindow::OSC_raw(const char *msg)
{
    if(!strcmp(msg, "/show"))
	show();
}


// TODO: change name
template<class T> void addMdiWindow(T*& subWidget, const QtOscNode* root, const char* _loc, QMdiArea* mdiArea, QMdiSubWindow*& window, /*T* widget,*/ bool visible)
{
	if(!subWidget)
		subWidget = new T(); // TODO: name
	if(!window)  {
		window = mdiArea->addSubWindow(subWidget);
	}
	window->setVisible(visible);
}

/*template<class T> T* new_if_null(T*& ptr) {
	return T ? T : new T();
}*/
#include <QDirIterator>

#include <QFile>
MainWindow::MainWindow(Fl_Osc_Interface *osc, QWidget *parent) :
	QMainWindow(parent),
//	QtOscObject(NULL, "/", this),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionToggle_AddSynth, SIGNAL(triggered(bool)), this, SLOT(toggleAddSynth(bool)));
	connect(ui->actionToggle_Piano, SIGNAL(triggered(bool)), this, SLOT(togglePiano(bool)));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
	connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(helpAboutQt()));
	connect(ui->actionFullscreen, SIGNAL(triggered(bool)), this, SLOT(toggleFullScreen(bool)));
	connect(ui->actionShow_Parameter_Tree, SIGNAL(triggered(bool)), this, SLOT(toggleParameterTree(bool)));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

//	qDebug() << QString::fromUtf8(":/icons/piano");


	 QDirIterator it(":", QDirIterator::Subdirectories);
while (it.hasNext()) {
    qDebug() << it.next();
}

	// this is a dumb fix for a current bug ... (TODO)
	ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	/*if(!QFile::exists(":/icons/piano"))
	 exit(99);
*/
	toggleParameterTree(false);
	toggleAddSynth(false);
	togglePiano(false);

#ifdef EMBEDDED_IN_LMMS
	// LMMS brins its own piano below our GUI
	ui->actionToggle_Piano->setVisible(false);
#endif

	/*
	 * Now that all widgets are created, this will init the root node
	 * and then recurse on all widgets.
	 */
    // TODO: mainWindow might be the only QtOscNode which
    // has a nullpointer as Fl_Osc_Interface.
	makeRoot(this, osc);
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::toggleAddSynth(bool visible)
{
/*	if(!addSynthWindow){
		AddSynthWidget* addSynthWidget = new AddSynthWidget();
		addSynthWindow = ui->mdiArea->addSubWindow(addSynthWidget);
	}
	addSynthWindow->setVisible(visible);*/
	addMdiWindow<AddSynthWidget>(addSynth, this, "/part1/kit1/adpars/voice1/", ui->mdiArea, addSynthWindow, visible);
}

void MainWindow::togglePiano(bool visible)
{
	Q_UNUSED(visible);
	addMdiWindow<VkWidget>(vkWidget, this, "", ui->mdiArea, vkWindow, visible);
	/*if(!pianoWindow){
		AddSynthWidget* addSynthWidget = new AddSynthWidget();
		addSynthWindow = ui->mdiArea->addSubWindow(addSynthWidget);
	}
	addSynthWindow->setVisible(b);*/
}

void MainWindow::makeAllChildren(QtOscNode *dest, const char *_loc)
{
	makeChild(this, "show");
	makeChild(addSynth, "part1/kit1/adpars/voice1/");
	makeChild(treeWidget, "");
	makeChild(vkWidget, "");
	makeChild(ui->widget_3, "vu-meter");
	//makeChild(&pseudoVu, "vu-meter");
}

void MainWindow::helpAbout()
{
	QMessageBox::about ( NULL, "About - ZynAddSubFX",
				"<h1>ZynAddSubFX</h1>"
				"<i>(c) 20XX-20XX</i><br/>"
				"by TODO<br/><br/>"
				"<a href=\"https://TODO.com/TODO\">https://TODO.com/TODO</a>");

}

void MainWindow::helpAboutQt() {
	QMessageBox::aboutQt ( NULL, tr("About - Qt") );
}

void MainWindow::close()
{
	QMainWindow::close();
}

void MainWindow::toggleParameterTree(bool visible)
{
	addMdiWindow<TreeWidget>(treeWidget, this, "", ui->mdiArea, treeDlg, visible);
}
