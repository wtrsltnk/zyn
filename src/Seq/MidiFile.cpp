/*
  ZynAddSubFX - a software synthesizer

  MidiFile.cpp - MIDI file loader
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

#include <cstdio>
#include <cstring>
#include <stdint.h>//upgrade to cstdint when C++0x happens
#include "MidiFile.h"
#include "MidiEvents.h"
#include "../globals.h"

static const bool verbose = false;

static const double tick = 0.05; //how many seconds one tick has

static MidiEvents *me = NULL;
static FILE *file     = NULL;

//returns -1 if there is an error, otherwise 0
int loadfile(const char *filename);
int parsemidifile(MidiEvents *me_);
int parsetrack(int ntrack);

/* Low Level MIDI functions */

//get a byte from the midifile
uint8_t getbyte();
//peek the current byte from the midifile
uint8_t peekbyte();
//get a set of 4 bytes from the midifile
uint32_t getint32();
//get a word of 2 bytes from the midifile
uint16_t getint16();
//read a variable length quantity
uint32_t getvarint32();
//skip n bytes in file
void skipnbytes(unsigned int n);

int loadMidi(const char *file, class MidiEvents &midi)
{
    if(loadfile(file))
        return -1;

    midi.clear();

    if(parsemidifile(&midi))
        return -1;

    midi.finishRecord();

    return 0;
}

int loadfile(const char *filename)
{
    file = fopen(filename, "r");
    if(file == NULL)
        return -1;

    char header[4];
    ZERO(header, 4);
    fread(header, 4, 1, file);

    //test to see if this a midi file
    if((header[0] != 'M') || (header[1] != 'T') || (header[2] != 'h')
       || (header[3] != 'd')) {
        fclose(file);
        return -1;
    }
    rewind(file);

    return 0;
}

int parsemidifile(MidiEvents *me_)
{
    me = me_;

    //read the header
    const int chunk = getint32(); //MThd
    if(chunk != 0x4d546864)
        return -1;
    const int size  = getint32();
    if(size != 6)
        return -1; //header is always 6 bytes long

    const int format = getint16();
    if(verbose)
        printf("format %d\n", format);

    int ntracks = getint16(); //this is always 1 if the format is "0"
    if(verbose)
        printf("ntracks %d\n", ntracks);

    const int division = getint16();
    if(verbose)
        printf("division %d\n", division);

    if(division >= 0) { //delta time units in each a quater note
        //tick=???;
    }
    else    //SMPTE (frames/second and ticks/frame)
        fprintf(stderr, "ERROR: %s %s - SMPTE not implemented yet.\n", __FILE__, __FUNCTION__);

    if(ntracks >= NUM_MIDI_TRACKS)
        ntracks = NUM_MIDI_TRACKS - 1;

    for(int n = 0; n < ntracks; n++)
        if(parsetrack(n) < 0)
            return -1;

    if(verbose)
        printf("\nMIDI file succesfully parsed.\n");

    fclose(file);
    me = NULL;
    return 0;
}

//Parse various events
void parsenoteoff(char ntrack, char chan, unsigned int dt);
void parsenoteon(char ntrack, char chan, unsigned int dt);
void parsecontrolchange(char ntrack, char chan, unsigned int dt);
void parsepitchwheel(char ntrack, char chan, unsigned int dt);
void parsemetaevent(unsigned char mtype, unsigned char mlength);

//Add delay
void add_dt(char ntrack, unsigned int dt);

//convert the delta-time to internal format
unsigned int convertdt(unsigned int dt);

int parsetrack(int ntrack)
{
    if(verbose)
        printf("\n--==*Reading track %d **==--\n", ntrack);

    const int chunk = getint32(); //MTrk
    if(chunk != 0x4d54726b)
        return -1;

    const int size = getint32();
    if(verbose)
        printf("size = %d\n", size);

    const int oldPosition = ftell(file);

    unsigned char lastmsg = 0;
    unsigned int  dt      = 0;

    while(!feof(file)) {
        unsigned int msgdeltatime = getvarint32();

///	printf("MSGDELTATIME = %d\n",msgdeltatime);

//	dt+=msgdeltatime;

        int msg = peekbyte();
///	printf("raw msg=0x%x     ",msg);
        if(msg < 0x80)
            msg = lastmsg;
        else {
            lastmsg = msg;
            getbyte();
        }
///	printf("msg=0x%x\n",msg);

//	dt+=msgdeltatime;
        add_dt(ntrack, msgdeltatime);

        unsigned int mtype, mlength;

        switch(msg) {
        case 0x80 ... 0x8f: //note on off
            parsenoteoff(ntrack, msg & 0x0f, dt);
            dt = 0;
            break;
        case 0x90 ... 0x9f: //note on (or note off)
            parsenoteon(ntrack, msg & 0x0f, dt);
            dt = 0;
            break;
        case 0xa0 ... 0xaf: //aftertouch - ignored
            skipnbytes(2);
            break;
        case 0xb0 ... 0xbf: //control change
            parsecontrolchange(ntrack, msg & 0x0f, dt);
            dt = 0;
            break;
        case 0xc0 ... 0xcf: //program change - ignored
            skipnbytes(1);
            break;
        case 0xd0 ... 0xdf: //channel pressure - ignored
            skipnbytes(1);
            break;
        case 0xe0 ... 0xef: //channel mode messages
            skipnbytes(2);
            break;
        case 0xf0: //sysex - ignored
            while(getbyte() != 0xf7) {
                if(feof(file))
                    break;
            }
            break;
        case 0xf7: //sysex (another type) - ignored
            skipnbytes(getvarint32());
            break;

        case 0xff: //meta-event
            mtype   = getbyte();
            mlength = getbyte();
            parsemetaevent(mtype, mlength);
            break;

        default:
            getbyte();
            fprintf(stderr,"UNKNOWN message! 0x%x\n", msg);
            return -1;
            break;
        }


        if(feof(file))
            return -1;

        if((ftell(file) - oldPosition) == size)
            break;
        else
            if((ftell(file) - oldPosition) > size)
                return -1;
//    if (size!=6) return(-1);//header is always 6 bytes long
    }

    if(verbose)
        printf("End Track\n\n");

    return 0;
}


void parsenoteoff(char ntrack, char chan, unsigned int dt)
{
    const char note = getbyte();
    getbyte(); //get noteoff velocity - unused by zynaddsubfx

    if(chan >= NUM_MIDI_CHANNELS)
        return;

    if(verbose)
        printf("Note off:%d \n", note);
    me->writeevent(ntrack, SeqEvent::note(convertdt(dt), chan, note));
}


void parsenoteon(char ntrack, char chan, unsigned int dt)
{
    const char note = getbyte(), vel = getbyte();

    if(chan >= NUM_MIDI_CHANNELS)
        return;

    if(verbose)
        printf("[dt %d ]  Note on:%d %d\n", dt, note, vel);
    me->writeevent(ntrack, SeqEvent::note(convertdt(dt), chan, note, vel));
}

void parsecontrolchange(char ntrack, char chan, unsigned int dt)
{
    const char control = getbyte(), value = getbyte();

    if(chan >= NUM_MIDI_CHANNELS)
        return;

    if(verbose)
        printf("[dt %d] Control change:%d %d\n", dt, control, value);
    me->writeevent(ntrack, SeqEvent::control(convertdt(dt), chan, control, value));
}

void parsepitchwheel(char ntrack, char chan, unsigned int dt)
{
    const char vallo = getbyte(), valhi = getbyte();

    if(chan >= NUM_MIDI_CHANNELS)
        return;

    int value = (int)valhi * 128 + vallo;

    if(verbose)
        printf("[dt %d] Pitch wheel:%d\n", dt, value);
}

void parsemetaevent(unsigned char mtype, unsigned char mlength)
{
    if(verbose)
        printf("meta-event type=0x%x  length=%d\n", mtype, mlength);
    skipnbytes(mlength);
}

void add_dt(char ntrack, unsigned int dt)
{
    if(dt == 0)
        return;
    if(verbose)
        printf("delay-marker track=%d length=%d\n", ntrack, dt);
    me->writeevent(ntrack, SeqEvent::time(convertdt(dt)));
}

unsigned int convertdt(unsigned int dt)
{
    return dt * 15.0;
}

uint8_t getbyte()
{
    int byte = fgetc(file);
    return byte == EOF ? 0 : byte;
}

uint8_t peekbyte()
{
    int byte = ungetc(fgetc(file), file);
    return byte == EOF ? 0 : byte;
}

uint32_t getint32()
{
    uint32_t result = 0;
    for(int i = 0; i < 4; i++)
        result = result * 256 + getbyte();

    return feof(file) ? 0 : result;
}

uint16_t getint16()
{
    uint16_t result = 0;
    for(int i = 0; i < 2; i++)
        result = result * 256 + getbyte();

    return feof(file) ? 0 : result;
}

uint32_t getvarint32()
{
    uint32_t result = 0;
    uint8_t b;

    if((result = getbyte()) & 0x80) {
        result &= 0x7f;
        do {
            b      = getbyte();
            result = (result << 7) + (b & 0x7f);
        } while(b & 0x80);
    }

    return result;
}

void skipnbytes(unsigned int n)
{
    fseek(file, n, SEEK_CUR);
}

