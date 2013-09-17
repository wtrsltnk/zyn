#include <QApplication>

#include "mainwindow.h"
#include "MasterUI.h"

QApplication* MasterUI::a;

MasterUI::MasterUI(int *exitprogram_, Fl_Osc_Interface*&)
{
	w = new MainWindow();
	w->show();
}

void MasterUI::do_new_master_unconditional()
{
}

int MasterUI::do_load_master_unconditional(const char *filename, const char *display_name)
{
}

void MasterUI::do_save_master(const char *file)
{
}

void MasterUI::init(int &argc, char **argv)
{
	a = new QApplication(argc, argv);
}
