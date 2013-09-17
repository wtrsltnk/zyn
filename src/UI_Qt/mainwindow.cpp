#include <QMdiSubWindow>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addsynthwidget.h"


template<class T> void pack(QMdiArea* mdiArea, QMdiSubWindow*& window, /*T* widget,*/ bool visible)
{
	if(!window)  {
		T* addSynthWidget = new T(); // TODO: name
		window = mdiArea->addSubWindow(addSynthWidget);
	}
	window->setVisible(visible);
}

/*template<class T> T* new_if_null(T*& ptr) {
	return T ? T : new T();
}*/

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	addSynthWindow(NULL),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionToggle_AddSynth, SIGNAL(triggered(bool)), this, SLOT(toggleAddSynth(bool)));
	connect(ui->actionToggle_Piano, SIGNAL(triggered(bool)), this, SLOT(togglePiano(bool)));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
	connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(helpAboutQt()));
	connect(ui->actionFullscreen, SIGNAL(triggered(bool)), this, SLOT(toggleFullScreen(bool)));
	connect(ui->actionShow_Parameter_Tree, SIGNAL(triggered(bool)), this, SLOT(toggleParameterTree(bool)));
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
	pack<AddSynthWidget>(ui->mdiArea, addSynthWindow, visible);
}

void MainWindow::togglePiano(bool visible)
{
	Q_UNUSED(visible);
	/*if(!pianoWindow){
		AddSynthWidget* addSynthWidget = new AddSynthWidget();
		addSynthWindow = ui->mdiArea->addSubWindow(addSynthWidget);
	}
	addSynthWindow->setVisible(b);*/
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
}

void MainWindow::toggleParameterTree(bool visible)
{
	pack<TreeDialog>(ui->mdiArea, treeDlg, visible);
}

/*template<class T> pack(QMdiArea* mdiArea, QMdiSubWindow* window, T* widget, bool visible)
{
	if(!window)  {
		T* addSynthWidget = new T(); // TODO: name
		window = mdiArea->addSubWindow(addSynthWidget);
	}
}*/
/*
void MainWindow::toggleWin(QWidget* content, bool visible)
{
	if(!addSynthWindow)  {
		AddSynthWidget* addSynthWidget = new AddSynthWidget();
		addSynthWindow = ui->mdiArea->addSubWindow(addSynthWidget);
	}
	addSynthWindow->setVisible(visible);
}*/
