#ifndef MASTERUI_H
#define MASTERUI_H

class Master;
class MainWindow;
class QApplication;

typedef int FL_Widget;
#include "../UI/Fl_Osc_Interface.h"

/**
 * @brief The MasterUI class
 * A dummy for Qt, I don't know what it actually needs.
 */
class MasterUI
{
	MainWindow* w;
// TODO: remove unnecessary funcs
public:
	static QApplication* a; // TODO: make private, processEvents func
/*	void updatesendwindow();
	void updatepanel();
	void setfilelabel(const char *filename);*/
	static void init(int & argc, char ** argv);
	MasterUI(int *exitprogram_, Fl_Osc_Interface*&);
	inline ~MasterUI() {}
/*	void showUI();
	void simplerefresh();*/
	void do_new_master_unconditional();
//	void do_new_master();
	int do_load_master_unconditional(const char *filename, const char *display_name);
//	void do_load_master(const char* file = NULL);*/
	void do_save_master(const char* file = NULL); // TODO: not inline
//	void refresh_master_ui();
};


#endif // MASTERUI_H
