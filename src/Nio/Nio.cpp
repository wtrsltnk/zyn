#include "Nio.h"
#include "OutMgr.h"
#include "InMgr.h"
#include "EngineMgr.h"
#include "MidiIn.h"
#include "AudioOut.h"
#include "../Misc/osc.h"
#include <iostream>
using namespace std;

Nio &Nio::getInstance()
{
    static Nio instance;
    return instance;
}

static int source_handler(const char *path, const char *types, lo_arg **argv, int argc,
        void *data, void *user_data)
{
    bool res = static_cast<Nio*>(user_data)->setSource(&argv[0]->s);
    lo_send(osc::ui, "/nio/source", res ? "T" : "F");
}

static int sink_handler(const char *path, const char *types, lo_arg **argv, int argc,
        void *data, void *user_data)
{
    bool res = static_cast<Nio*>(user_data)->setSink(&argv[0]->s);
    lo_send(osc::ui, "/nio/sink", res ? "T" : "F");
}

Nio::Nio()
:autoConnect(false),
    in(InMgr::getInstance()),//Enable input wrapper
    out(OutMgr::getInstance()),//Initialize the Output Systems
    eng(EngineMgr::getInstance()),//Initialize The Engines
    postfix("")//no default postfix
{
    lo_server_add_method(osc::back_server, "/nio/source", "s", source_handler, this);
    lo_server_add_method(osc::back_server, "/nio/sink",   "s", sink_handler,   this);
}

Nio::~Nio()
{
    stop();
}

bool Nio::start()
{
    return eng.start();//Drivers start your engines!
}

void Nio::stop()
{
    eng.stop();
}
    
int Nio::setDefaultSource(string name)
{
    if(name.empty())
        return 0;

    if(!eng.setInDefault(name)) {
        cerr << "There is no input for " << name << endl;
        return false;
    }
    return 0;
}
    

int Nio::setDefaultSink(string name)
{
    if(name.empty())
        return 0;

    if(!eng.setOutDefault(name)) {
        cerr << "There is no output for " << name << endl;
    }
    return 0;
}

bool Nio::setSource(string name)
{
     return in.setSource(name);
}

bool Nio::setSink(string name)
{
     return out.setSink(name);
}

void Nio::setPostfix(std::string post)
{
    postfix = post;
}

std::string Nio::getPostfix(void) const
{
    return postfix;
}

        
set<string> Nio::getSources() const
{
    set<string> sources;
    for(list<Engine *>::iterator itr = eng.engines.begin();
            itr != eng.engines.end(); ++itr)
        if(dynamic_cast<MidiIn *>(*itr))
            sources.insert((*itr)->name);
    return sources;
}

set<string> Nio::getSinks() const
{
    set<string> sinks;
    for(list<Engine *>::iterator itr = eng.engines.begin();
            itr != eng.engines.end(); ++itr)
        if(dynamic_cast<AudioOut *>(*itr))
            sinks.insert((*itr)->name);
    return sinks;
}
        
string Nio::getSource() const
{
     return in.getSource();
}

string Nio::getSink() const
{
     return out.getSink();
}

