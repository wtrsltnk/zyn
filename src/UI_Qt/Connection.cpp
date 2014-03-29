#include "../UI/Connection.h"
typedef int FL_Widget;
#include "../UI/Fl_Osc_Interface.h"
#include "../globals.h"

#include <QApplication>
#include <rtosc/rtosc.h>
#include <rtosc/ports.h>

#include <rtosc/include/rtosc/ports.h>

//#include <FL/Fl.H>
//#include "Fl_Osc_Tree.H"
#include "../src/Misc/Master.h"
#include "../UI/common.H"
#include "MasterUI.h"

#include "mainwindow.h"

using namespace GUI;
MasterUI *ui;

Fl_Osc_Interface *osc;//TODO: the scope of this should be narrowed

#ifdef NTK_GUI
static Fl_Tiled_Image *	;
#endif

/*void
set_module_parameters ( Fl_Widget *o )
{
#ifdef NTK_GUI
    o->box( FL_DOWN_FRAME );
    o->align( o->align() | FL_ALIGN_IMAGE_BACKDROP );
    o->color( FL_BLACK );
    o->image( module_backdrop );
    o->labeltype( FL_SHADOW_LABEL );
#else
    o->box( FL_PLASTIC_UP_BOX );
    o->color( FL_CYAN );
    o->labeltype( FL_EMBOSSED_LABEL );
#endif
}*/

void GUI::initUi(int & argc, char ** argv)
{
	ui->init(argc, argv);
}

ui_handle_t GUI::createUi(Fl_Osc_Interface *osc, void *exit)
{
	::osc = osc;
    return (void*)(ui = new MasterUI((int*)exit, osc));
/*
    ::osc = osc;

    Fl_Window *midi_win = new Fl_Double_Window(400, 400, "Midi connections");
    Fl_Osc_Tree *tree   = new Fl_Osc_Tree(0,0,400,400);
    midi_win->resizable(tree);
    tree->root_ports    = &Master::ports;
    tree->osc           = osc;
    midi_win->show();

    return (void*) (ui = new MasterUI((Master*)master, (int*)exit, osc));
    */
}

void GUI::destroyUi(ui_handle_t _ui)
{
	MasterUI* ui = static_cast<MasterUI*>(_ui);
	ui->quit();
	delete ui;
}


#define BEGIN(x) {x,"",NULL,[](const char *m, rtosc::RtData d){ \
    MasterUI *ui   = static_cast<MasterUI*>(d.obj); \
    rtosc_arg_t a0 = rtosc_argument(m,0);
/*    //rtosc_arg_t a1 = rtosc_argument(m,1); \
    //rtosc_arg_t a2 = rtosc_argument(m,2); \
    //rtosc_arg_t a3 = rtosc_argument(m,3);*/
#define END }},

#include <QMessageBox>
inline void fl_alert(const char* text) {
	QMessageBox::warning(NULL, "Alert!", text);
}

//DSL based ports
static rtosc::Ports ports = {
    BEGIN("show:T") {
	ui->showUI();
    } END
    BEGIN("alert:s") {
	fl_alert(a0.s);
    } END
    BEGIN("session-type:s") {
	if(strcmp(a0.s,"LASH"))
	    return;
	ui->sm_indicator1->value(1);
	ui->sm_indicator2->value(1);
	ui->sm_indicator1->tooltip("LASH");
	ui->sm_indicator2->tooltip("LASH");
    } END
    BEGIN("save-master:s") {
	ui->do_save_master(a0.s);
    } END
    BEGIN("load-master:s") {
	ui->do_load_master(a0.s);
    } END
    BEGIN("vu-meter:bb") {
	rtosc_arg_t a1 = rtosc_argument(m,1);
	if(a0.b.len == sizeof(vuData) &&
		a1.b.len == sizeof(float)*NUM_MIDI_PARTS) {
	    //Refresh the primary VU meters
	    ui->simplemastervu->update((vuData*)a0.b.data);
	    ui->mastervu->update((vuData*)a0.b.data);

	    float *partvu = (float*)a1.b.data;
	    for(int i=0; i<NUM_MIDI_PARTS; ++i)
		ui->panellistitem[i]->partvu->update(partvu[i]);
	}
    } END
};

//! is being called by the UI thread. deprecated.
void GUI::raiseUi(ui_handle_t gui, const char *message)
{
	(void)gui;
	printf("got message for UI '%s'\n", message);
	MasterUI *mui = (MasterUI*)gui;
    mui->osc->tryLink(message); // TODO: must we implement this?
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	rtosc::RtData d;
	d.loc = buffer;
	d.loc_size = 1024;
    d.obj = gui;
    ports.dispatch(message+1, d);
}

void GUI::raiseUi(ui_handle_t gui, const char *dest, const char *args, ...)
{
	char buffer[1024];
	va_list va;
	va_start(va,args);
	if(rtosc_vmessage(buffer,1024,dest,args,va))
	 raiseUi(gui, buffer);
}

#include <QApplication>

void GUI::tickUi(ui_handle_t)
{
	ui->appli->processEvents(QEventLoop::AllEvents, 20);
}

void GUI::loopUi(MiddleWare* _middleware, LASHClient* _lash,
	NSM_Client* _nsm)
{
	ui->run_loop(_middleware, _lash, _nsm);
}


