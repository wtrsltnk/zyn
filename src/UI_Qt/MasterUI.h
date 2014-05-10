#ifndef MASTERUI_H
#define MASTERUI_H

#include <QObject>
#include <QTimer>
#include <QThread>

class Master;
class MainWindow;
class QApplication;
class vuData;
class QThread;

typedef int FL_Widget;
#include "../UI/Fl_Osc_Interface.h"
#include "../globals.h"

class MiddleWare;

#if LASH
class LASHClient;
#else
typedef void LASHClient;
#endif

#if USE_NSM
class NSM_Client;
#else
typedef void NSM_Client;
#endif

class ThreadLinkInterface;

// TODO: move this somewhere else
//! This class handles MiddleWare's ticks
class nonGuiThreadT : public QThread
{
    Q_OBJECT

    // TODO: use refs here if possible?
    class MiddleWare* middleware;

    #if LASH
    LASHClient* lash;
    #else
    void* lash;
    #endif

    #if USE_NSM
    NSM_Client *nsm = 0;
    #else
    void* nsm;
    #endif

    Fl_Osc_Interface* osc;
    int* const exitprogram;
    //QTimer nonGuiTimer;

    void callback();
public:
    void run() {
        //nonGuiTimer.start(20);
        while(! *exitprogram) {
            callback();
            QThread::msleep(20);
        }
    }

    void init(MiddleWare* _middleware, LASHClient *_lash, NSM_Client* _nsm) {
        middleware = _middleware;
        lash = _lash;
        nsm = _nsm;
    }

    nonGuiThreadT(Fl_Osc_Interface* osc, int* exitprogram) :
        osc(osc),
        exitprogram(exitprogram) {
        //connect(&nonGuiTimer, SIGNAL(timeout()), this, SLOT(callback()));
    }
};

/**
 * @brief The MasterUI class
 * A dummy for Qt, I don't know what it actually needs.
 */
class MasterUI : public QObject
{
	Q_OBJECT

	MainWindow* w;
    int* const exitprogram;

	nonGuiThreadT nonGuiThread;
	QTimer linkFetchTimer;

// data
    Fl_Osc_Interface *osc;
	static QApplication* appli; // TODO: make private, processEvents func

	void showUI();
	void simplerefresh();

/*	void updatesendwindow();
	void updatepanel();
	void setfilelabel(const char *filename);*/

private slots:
	void request_exit();
	void linkFetch() { osc->tryLinkUi(); }
public:
	void init(int & argc, char ** argv);
    MasterUI(int *exitprogram_, Fl_Osc_Interface *_osc);
	~MasterUI();
	void run_loop(MiddleWare* _middleware, LASHClient* _lash,
		NSM_Client* _nsm);
	void quit();
	void raise(const char *message) { osc->tryLink(message); }

public: // callbacks for NSM
	void do_new_master_unconditional();
//	void do_new_master();
	int do_load_master_unconditional(const char *filename, const char *display_name);
	void do_load_master(const char* file = NULL); //
	void do_save_master(const char* file = NULL); // TODO: not inline
//	void refresh_master_ui();
};


#endif // MASTERUI_H
