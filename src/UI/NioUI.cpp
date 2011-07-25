#include "NioUI.h"
#include "../Nio/Nio.h"
#include "../Misc/osc.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Spinner.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Display.H>

using namespace std;

static int error_handle(const char *path, const char *types, lo_arg **argv, int argc,
        void *data, void *user_data)
{
    Fl_Choice *ch = static_cast<Fl_Choice *>(user_data);
    ch->textcolor(fl_rgb_color(255,0,0));
    ch->redraw();
}

NioUI::NioUI()
    :Fl_Window(200,100,400,400,"New IO Controls")
{
    //hm, I appear to be leaking memory
    Fl_Tabs *wintabs = new Fl_Tabs(0,0,400,400-15);
    {
        Fl_Group *gen  = new Fl_Group(0,20,400,400-35,"General");
        {
            Fl_Text_Buffer *buff  = new Fl_Text_Buffer();
            Fl_Text_Display *intro = new Fl_Text_Display(20,40,350,300);
            intro->buffer(buff);
            buff->text("Thanks For Testing Out the New"
                    " Input/Output system. "
                    "Beware of bugs that may exist and"
                    " enjoy the new system.");
            intro->wrap_mode(4, 40);
        }
        gen->end();

        Fl_Group *settings = new Fl_Group(0,20,400,400-35,"Settings");
        {
            audio = new Fl_Choice(60, 80, 100, 25, "Audio");
            audio->callback(audioCallback);
            midi = new Fl_Choice(60, 100, 100, 25, "Midi");
            midi->callback(midiCallback);
        }
        settings->end();

        Nio &nio = Nio::getInstance();

        //initialize midi list
        {
            set<string> midiList = nio.getSources();
            string source = nio.getSource();
            int midival = 0;
            for(set<string>::iterator itr = midiList.begin();
                    itr != midiList.end(); ++itr) {
                midi->add(itr->c_str());
                if(*itr == source)
                    midival = midi->size() - 2;
            }
            midi->value(midival);
        }

        //initialize audio list
        {
            set<string> audioList = nio.getSinks();
            string sink = nio.getInstance().getSink();
            int audioval = 0;
            for(set<string>::iterator itr = audioList.begin();
                    itr != audioList.end(); ++itr) {
                audio->add(itr->c_str());
                if(*itr == sink)
                    audioval = audio->size() - 2;
            }
            audio->value(audioval);
        }
    }
    wintabs->end();

    //add error callbacks
    lo_server_thread_add_method(osc::ui_server, "/nio/source", "F", error_handle, midi);
    lo_server_thread_add_method(osc::ui_server, "/nio/sink",   "F", error_handle, audio);

    resizable(this);
    size_range(400,300);
}

NioUI::~NioUI()
{
}

void NioUI::refresh()
{
}

void NioUI::midiCallback(Fl_Widget *c)
{
    Fl_Choice *ch = static_cast<Fl_Choice *>(c);
    ch->textcolor(fl_rgb_color(0,0,0));
    lo_send(osc::backend, "/nio/source", "s", ch->text());

}

void NioUI::audioCallback(Fl_Widget *c)
{
    Fl_Choice *ch = static_cast<Fl_Choice *>(c);
    ch->textcolor(fl_rgb_color(0,0,0));
    lo_send(osc::backend, "/nio/sink", "s", ch->text());
}

