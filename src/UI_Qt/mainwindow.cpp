#include <QMdiSubWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addsynthwidget.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	addSynthWindow(NULL),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionToggle_AddSynth, SIGNAL(triggered(bool)), this, SLOT(toggleAddSynth(bool)));
	connect(ui->actionToggle_Piano, SIGNAL(triggered(bool)), this, SLOT(togglePiano(bool)));
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::toggleAddSynth(bool visible)
{
	if(!addSynthWindow){
		AddSynthWidget* addSynthWidget = new AddSynthWidget();
		addSynthWindow = ui->mdiArea->addSubWindow(addSynthWidget);
	}
	addSynthWindow->setVisible(visible);
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

