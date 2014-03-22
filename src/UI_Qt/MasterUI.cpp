#include <QDebug> // TODO
#include <QApplication>

#include "Misc/MiddleWare.h"

#if LASH
#include "Misc/LASHClient.h"
#endif

#if USE_NSM
#include "UI/NSM.H"
#endif

#include "mainwindow.h"
#include "MasterUI.h"

ThreadLinkInterface tli; // TODO: a better place for this?

//! This is needed since we have a gui library.
//! The macro must be outside a namespace (and maybe class).
inline void initMyResource() { Q_INIT_RESOURCE(ressources_qt); }

void nonGuiThreadT::callback()
{
    // this is copied from main.cpp
    // TODO: in future, move this to a common header
    #if USE_NSM
    if(nsm) {
        nsm->check();
        goto done;
    }
    #endif
    #if LASH
    {
        string filename;
        switch(lash->checkevents(filename)) {
            case LASHClient::Save:
                GUI::raiseUi(gui, "/save-master", "s", filename.c_str());
                lash->confirmevent(LASHClient::Save);
                break;
            case LASHClient::Restore:
                GUI::raiseUi(gui, "/load-master", "s", filename.c_str());
                lash->confirmevent(LASHClient::Restore);
                break;
            case LASHClient::Quit:
                Pexitprogram = 1;
            default:
                break;
        }
    }
    #endif //LASH

    #if USE_NSM
    done:
    #endif
        middleware->tick();
        osc->tryLinkUi();

}

QApplication* MasterUI::appli = NULL;

MasterUI::MasterUI(int *exitprogram_, Fl_Osc_Interface *_osc) :
    exitprogram(exitprogram_),
    nonGuiThread(_osc),
    sm_indicator1(new Fl_Button),
    sm_indicator2(new Fl_Button),
    mastervu(new VuMasterMeter),
    simplemastervu(new VuMasterMeter),
    osc(_osc)
{
    for (int i=0;i<NUM_MIDI_PARTS;i++){ panellistitem[i]=new Panellistitem; panellistitem[i]->init(i,NULL); }

    w = new MainWindow(&tli);
//    connect(&nonGuiTimer, SIGNAL(timeout()), this, SLOT(do_non_gui_stuff()));
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
    appli->quit();
    //delete a;
}

void MasterUI::init(int &argc, char **argv)
{
    if(!appli)
     appli = new QApplication(argc, argv);
    appli->setQuitOnLastWindowClosed(true);
    connect(appli, SIGNAL(lastWindowClosed()), this, SLOT(request_exit()));
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

void MasterUI::run_loop(MiddleWare* _middleware, LASHClient *_lash, NSM_Client* _nsm)
{
    // prepare non gui
//    nonGuiThread.middleware = _middleware;
//    lash = _lash;
//    nsm = _nsm;
    //nonGuiTimer.start(20);

    nonGuiThread.init(_middleware, _lash, _nsm);
    nonGuiThread.start();

    // run gui
    /*return TODO */ appli->exec();
}

void MasterUI::simplerefresh()
{
    // shall refresh the group of widgets for the simple ui
}

#if 0
void MasterUI::do_non_gui_stuff()
{
    // this is copied from main.cpp
    // TODO: in future, move this to a common header
    #if USE_NSM
    if(nsm) {
        nsm->check();
        goto done;
    }
    #endif
    #if LASH
    {
        string filename;
        switch(lash->checkevents(filename)) {
            case LASHClient::Save:
                GUI::raiseUi(gui, "/save-master", "s", filename.c_str());
                lash->confirmevent(LASHClient::Save);
                break;
            case LASHClient::Restore:
                GUI::raiseUi(gui, "/load-master", "s", filename.c_str());
                lash->confirmevent(LASHClient::Restore);
                break;
            case LASHClient::Quit:
                Pexitprogram = 1;
            default:
                break;
        }
    }
    #endif //LASH

    #if USE_NSM
    done:
    #endif
        middleware->tick();
}
#endif
