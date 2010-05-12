/*
  ZynAddSubFX - a software synthesizer

  MIDIEvents.cpp - It stores the midi events from midi file or sequencer
  Copyright (C) 2003-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "MIDIEvents.h"
using namespace std;

SeqEvent::SeqEvent()
    :deltatime(0)
{}

SeqEvent::SeqEvent(int dt, int type, int chan, int number, int val)
    :MidiEvent(chan, type, number, val), deltatime(dt)
{}


SeqEvent SeqEvent::note(int dt, int chan, int note, int velocity)
{
    return SeqEvent(dt, 1, chan, note, velocity);
}

SeqEvent SeqEvent::control(int dt, int chan, int ctl, int value)
{
    return SeqEvent(dt, 1, chan, ctl, value);
}

SeqEvent SeqEvent::time(int dt)
{
    return SeqEvent(dt, 255, 0, 0, 0);
}

MIDIEvents::MIDIEvents()
{}

MIDIEvents::~MIDIEvents()
{}


/************** Track stuff ***************/
void MIDIEvents::writeevent(unsigned int track, const SeqEvent &ev)
{
    miditrack[track].record.push_back(ev);
}

SeqEvent MIDIEvents::readevent(unsigned int track)
{
    //alias names for clarity
    iterator &itr = miditrack[track].track_itr;
    list &l       = miditrack[track].track;

    if(!l.size() || itr == l.end()) {
        SeqEvent ev;
        ev.type = -1;
        return ev;//Returning Quit Signal
    }

    return *(itr++);
}


void MIDIEvents::rewindlist(unsigned int track)
{
    miditrack[track].track_itr = miditrack[track].track.begin();
}

void MIDIEvents::clear()
{
    for(int i = 0; i < NUM_MIDI_TRACKS; i++) {
        miditrack[i].record.clear();
        miditrack[i].track.clear();
        miditrack[i].track_itr = miditrack[i].track.begin();
    }
}

void MIDIEvents::finishRecord()
{
    //copy the "record" track to the main track
    for(int i = 0; i < NUM_MIDI_TRACKS; i++) {
        miditrack[i].track = miditrack[i].record;
        miditrack[i].record.clear();
    }
}

