#include "MidiEvent.h"

using namespace std;

MidiEvent::MidiEvent()
    :channel(0),type(0),num(0),value(0)
{}

ostream &operator<<(ostream &out, const MidiEvent& ev)
{
    if(ev.type == M_NOTE)
        out << "MidiNote: note("     << ev.num      << ")\n"
            << "          channel("  << ev.channel  << ")\n"
            << "          velocity(" << ev.value    << ")";
    else
        out << "MidiCtl: controller(" << ev.num     << ")\n"
            << "         channel("    << ev.channel << ")\n"
            << "         value("      << ev.value   << ")";
    return out;
}
