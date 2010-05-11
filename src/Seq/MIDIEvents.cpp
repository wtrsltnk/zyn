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

MIDIEvents::MIDIEvents()
{}

MIDIEvents::~MIDIEvents()
{}


/************** Track stuff ***************/
void MIDIEvents::writeevent(unsigned int track, const event &ev)
{
    miditrack[track].record.push_back(ev);
}

struct MIDIEvents::event MIDIEvents::readevent(unsigned int track)
{
    //alias
    iterator &itr = miditrack[track].track_itr;
    list &l = miditrack[track].track;

    if(itr==l.end()) {
        event ev;
        ev.type = -1;
        return ev;
    }

    return *++itr;
}


void MIDIEvents::rewindlist(unsigned int track)
{
    miditrack[track].track_itr = miditrack[track].track.begin();
}

