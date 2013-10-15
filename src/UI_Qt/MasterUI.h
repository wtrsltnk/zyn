#ifndef MASTERUI_H
#define MASTERUI_H

#include <QObject>

class Master;
class MainWindow;
class QApplication;
class vuData;

typedef int FL_Widget;
#include "../UI/Fl_Osc_Interface.h"
#include "../globals.h"

/**
 * @brief The MasterUI class
 * A dummy for Qt, I don't know what it actually needs.
 */
class MasterUI : public QObject
{
	Q_OBJECT

	// TODO: deleta a and w?
	MainWindow* w;
	int* exitprogram;
// TODO: remove unnecessary funcs
private slots:
	void request_exit();
public:
// data
	struct Fl_Button { void value(int){} void tooltip(const char*){} }; // TODO: replace
	struct VuMasterMeter { void update(vuData*) {} }; // TODO: replace
	struct Panellistitem {
		struct VuPartMeter {
			void update(float) {}
		};
		VuPartMeter *partvu;
		void update(float*) {}
		void init(int, char*) { partvu = new VuPartMeter(); }
	}; // TODO: replace

	Fl_Button *sm_indicator1, *sm_indicator2;
	VuMasterMeter *mastervu, *simplemastervu;
	Panellistitem *panellistitem[NUM_MIDI_PARTS];
	class Fl_Osc_Interface *osc;

	static QApplication* a; // TODO: make private, processEvents func
/*	void updatesendwindow();
	void updatepanel();
	void setfilelabel(const char *filename);*/
// required
	static void init(int & argc, char ** argv);
	MasterUI(int *exitprogram_, Fl_Osc_Interface*&_osc);
	~MasterUI();
	void showUI();

	void simplerefresh();
	void do_new_master_unconditional();
//	void do_new_master();
	int do_load_master_unconditional(const char *filename, const char *display_name);
	void do_load_master(const char* file = NULL);
	void do_save_master(const char* file = NULL); // TODO: not inline
//	void refresh_master_ui();
// additional
	void quit();
};


#endif // MASTERUI_H
