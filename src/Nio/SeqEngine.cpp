/*
  ZynAddSubFX - a software synthesizer

  SeqEngine - Wrapper for midi sequencer
  Copyright (C) 2010-2010 Mark McCurry
  Author: Mark McCurry

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

#include "SeqEngine.h"
#include "InMgr.h"

using namespace std;

SeqEngine::SeqEngine()
    :engThread(NULL)
{
    name = "SEQ";
}

SeqEngine::~SeqEngine()
{
    Stop();
}

bool SeqEngine::Start()
{
    return true; 
}

void SeqEngine::Stop()
{
    stopMidi();
}

void SeqEngine::setMidiEn(bool nval)
{
    if(nval)
        openMidi();
    else
        stopMidi();
}

bool SeqEngine::getMidiEn() const
{
    return true;
}

bool SeqEngine::openMidi()
{

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    engThread = new pthread_t;
    pthread_create(engThread, &attr, _thread, this);

    return true;
}

void SeqEngine::stopMidi()
{
}

void *SeqEngine::_thread(void *arg)
{
    return (static_cast<SeqEngine*>(arg))->thread();
}

bool some(bool *array)
{
    bool ans = false;
    for(int i = 0; i < NUM_MIDI_TRACKS; i++)
        ans |= array[i];
    return ans;
}

void *SeqEngine::thread()
{
    string location = string(SOURCE_DIR) + string("/Tests/test.midi");
    Sequencer seq;
    seq.importmidifile(location.c_str());

    seq.startplay();
    bool tracks[NUM_MIDI_TRACKS];
    for(int i = 0; i < NUM_MIDI_TRACKS; i++)
        tracks[i] = true;

    while(some(tracks)) {
        for(int ntrack = 0; ntrack < NUM_MIDI_TRACKS; ntrack++) {
            if(!tracks[ntrack])
                continue;

            MidiEvent ev;
            seq.getevent(ntrack, ev);

            if(ev.type == -1) //track has finished
                tracks[ntrack] = false;

            if(ev.type == 1) //note_on or note_off
                sysIn->putEvent(ev);
        }
        usleep(1000);
    }

    pthread_exit(NULL);
    return NULL;
}

