//Defines the methods of communication for the GUI
//Expect this code to mutate into some sort of ugly beast that will slowly
//remove the tendrils of the UI from the RT code


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

class Fl_Osc_Interface;
namespace GUI
{
typedef void *ui_handle_t;

void initUi(int & argc, char ** argv);
void loopUi(MiddleWare*, LASHClient*, NSM_Client *_nsm);
ui_handle_t createUi(Fl_Osc_Interface *osc, void *exit);
void destroyUi(ui_handle_t _ui);
void raiseUi(ui_handle_t, const char *);
void raiseUi(ui_handle_t, const char *, const char *, ...);
void tickUi(ui_handle_t);
};
