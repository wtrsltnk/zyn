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

#ifndef SEQ_ENGINE_H
#define SEQ_ENGINE_H

#include "MidiIn.h"
#include "../Seq/Sequencer.h"

class SeqEngine: public MidiIn
{
    public:
        SeqEngine();
        ~SeqEngine();

        bool Start();
        void Stop();

        void setMidiEn(bool nval);
        bool getMidiEn() const;


    protected:
        void *thread();
        static void *_thread(void *arg);

    private:
        pthread_t *engThread;

        //Midi
        bool openMidi();
        void stopMidi();
};

#endif
