#include <QApplication>

#include "mainwindow.h"
#include "MasterUI.h"

QApplication* MasterUI::a;

MasterUI::MasterUI(int *exitprogram_, Fl_Osc_Interface*&)
{
	w = new MainWindow();
	w->show();

}

void MasterUI::init(int &argc, char **argv)
{
	a = new QApplication(argc, argv);
}
