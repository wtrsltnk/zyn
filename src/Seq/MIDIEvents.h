/*
  ZynAddSubFX - a software synthesizer

  MIDIEvents.h - It stores the midi events from midi file or sequencer
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
#ifndef MIDI_EVENTS_H
#define MIDI_EVENTS_H

#include <list>
#include "../globals.h"
#include "../Nio/InMgr.h" //this is for MidiEvent
#define NUM_MIDI_TRACKS NUM_MIDI_CHANNELS

struct SeqEvent: public MidiEvent
{
    SeqEvent();
    int deltatime;
};

/**storage the midi events from midi file or sequencer*/
class MIDIEvents
{
    public:
        MIDIEvents();
        ~MIDIEvents();

        void writeevent(unsigned int track, const SeqEvent &ev);
        SeqEvent readevent(unsigned int track);
        void rewindlist(unsigned int track);

        void clear();
        void finishRecord();

    private:
        typedef std::list<SeqEvent> list;
        typedef list::iterator iterator;

        struct {
            list track; //the stored track
            iterator track_itr;
            list record; //the track being "recorded" from midi
        } miditrack[NUM_MIDI_TRACKS];
};

#endif

