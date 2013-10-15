#include <QDebug> // TODO
#include <QApplication>

#include "mainwindow.h"
#include "MasterUI.h"

//! This is needed since we have a gui library.
//! The macro must be outside a namespace (and maybe class).
inline void initMyResource() { Q_INIT_RESOURCE(ressources_qt); }

QApplication* MasterUI::a;

MasterUI::MasterUI(int *exitprogram_, Fl_Osc_Interface*& _osc) :
	exitprogram(exitprogram_),
	sm_indicator1(new Fl_Button),
	sm_indicator2(new Fl_Button),
	mastervu(new VuMasterMeter),
	simplemastervu(new VuMasterMeter),
	osc(_osc)
{
	for (int i=0;i<NUM_MIDI_PARTS;i++){ panellistitem[i]=new Panellistitem; panellistitem[i]->init(i,NULL); }

	w = new MainWindow(osc);
	connect(a, SIGNAL(lastWindowClosed()), this, SLOT(request_exit()));
	simplerefresh(); // this behaviour is suggested
}

MasterUI::~MasterUI()
{
	//delete w;
	//*exitprogram = (int)true;
}

void MasterUI::do_new_master_unconditional()
{
}

int MasterUI::do_load_master_unconditional(const char *filename, const char *display_name)
{
	(void)filename; (void)display_name;
	return 0; // TODO
}

void MasterUI::do_load_master(const char *file)
{
	(void)file;
}

void MasterUI::do_save_master(const char *file)
{
	(void)file;
}

void MasterUI::request_exit()
{
	qDebug() << "EXIT";
	*exitprogram = (int)true;
}


void MasterUI::quit()
{
	a->quit();
	//delete a;
}

void MasterUI::init(int &argc, char **argv)
{
	a = new QApplication(argc, argv);
	a->setQuitOnLastWindowClosed(false);
	initMyResource(); // see above
}

void MasterUI::showUI()
{
	w->show();
	//a->exec();
	/*switch (config.cfg.UserInterfaceMode)
	{
		case 0:selectuiwindow->show();
		break;
		case 1:masterwindow->show();
		break;
		case 2:simplemasterwindow->show();
		break;
	};*/
}

void MasterUI::simplerefresh()
{
	// shall refresh the group of widgets for the simple ui
}
