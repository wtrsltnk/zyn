#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include <iostream>

enum midi_type{
    M_NOTE = 1,
    M_CONTROLLER = 2
};    //type=1 for note, type=2 for controller

struct MidiEvent
{
    MidiEvent();
    int channel; //the midi channel for the event
    int type;    //type=1 for note, type=2 for controller
    int num;     //note or contoller number
    int value;   //velocity or controller value
};

std::ostream &operator<<(std::ostream &out, const MidiEvent& ev);
#endif
