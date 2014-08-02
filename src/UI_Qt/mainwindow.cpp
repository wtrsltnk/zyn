//#include <QMdiSubWindow>
#include "qtoscnode.h"
#include <QMessageBox>
#include <QDebug> // TODO

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addsynthwidget.h"
#include "treewidget.h"
#include "vkwidget.h"
#include "ui_qt_config.h"

void MainWindow::OSC_raw(const char *msg)
{
    if(!strcmp(msg, "/show"))
	show();
}

// TODO: change name
template<class T, class QtClass> void toggleMdiWindow(T*& subWidget, QTabWidget* tabWidget, QtClass*& window, bool visible)
{
	if(!subWidget)
		subWidget = new T(); // TODO: name
	if(!window)  {
	//	window = mdiArea->addSubWindow(
	//		subWidget);
		window = new QtClass;
		tabWidget->addTab(subWidget, "Tab");
	}
	window->setVisible(visible);
}

#include <QDirIterator>

MainWindow::MainWindow(Fl_Osc_Interface *osc, QWidget *parent) :
	QMainWindow(parent),
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

// use this code if you miss files from qt, like ":/icons/piano"
/*	 QDirIterator it(":", QDirIterator::Subdirectories);
while (it.hasNext()) {
    qDebug() << it.next();
}*/

	// this is a dumb fix for a current bug ... (TODO)
//	ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//	ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

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
	toggleMdiWindow<AddSynthWidget>(addSynth, ui->tabWidget, addSynthWindow, visible);
}

void MainWindow::togglePiano(bool visible)
{
	Q_UNUSED(visible);
	toggleMdiWindow<VkWidget>(vkWidget, ui->tabWidget, vkWindow, visible);
}

void MainWindow::makeAllChildren()
{
	makeChild(this, "show");
	makeChild(addSynth, "part0/kit0/adpars/global/");
	makeChild(treeWidget, "");
	makeChild(vkWidget, "");
	makeChild(ui->widget_3, "vu-meter");
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
	toggleMdiWindow<TreeWidget>(treeWidget, ui->tabWidget, treeDlg, visible);
}
