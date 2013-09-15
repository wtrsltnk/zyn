#include <QApplication>
#include "mainwindow.h"

#ifdef HAVE_GUI_MAIN
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	
	return a.exec();
}
#endif
