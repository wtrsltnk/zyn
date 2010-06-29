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
#include <iostream>
#include "MidiFile.h"

using namespace std;

MidiFile::MidiFile()
    :me(NULL),tick(0.05)
{}

MidiFile::~MidiFile()
{}

int MidiFile::loadfile(const char *filename)
{
    clearmidifile();

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

int MidiFile::parsemidifile(MidiEvents *me_)
{
    me = me_;

    //read the header
    int chunk = getint32(); //MThd
    if(chunk != 0x4d546864)
        return -1;
    int size  = getint32();
    if(size != 6)
        return -1; //header is always 6 bytes long

    int format = getint16();
    printf("format %d\n", format);

    int ntracks = getint16(); //this is always 1 if the format is "0"
    printf("ntracks %d\n", ntracks);

    int division = getint16();
    printf("division %d\n", division);
    if(division >= 0) { //delta time units in each a quater note
        //tick=???;
    }
    else    //SMPTE (frames/second and ticks/frame)
        cerr << "ERROR: in MidiFile.cpp::parsemidifile() - SMPTE not implemented yet."
             << endl;

    if(ntracks >= NUM_MIDI_TRACKS)
        ntracks = NUM_MIDI_TRACKS - 1;

    for(int n = 0; n < ntracks; n++) {
        if(parsetrack(n) < 0) {
            clearmidifile();
            return -1;
        }
    }

    printf("\nMIDI file succesfully parsed.\n");

    fclose(file);
    me = NULL;
    return 0;
}

//private members


int MidiFile::parsetrack(int ntrack)
{
    printf("\n--==*Reading track %d **==--\n", ntrack);

    int chunk = getint32(); //MTrk
    if(chunk != 0x4d54726b)
        return -1;

    int size = getint32();
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
            printf("UNKNOWN message! 0x%x\n", msg);
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

    printf("End Track\n\n");

    return 0;
}


void MidiFile::parsenoteoff(char ntrack, char chan, unsigned int dt)
{
    unsigned char note;
    note = getbyte();
    getbyte(); //get noteoff velocity - unused by zynaddsubfx

    if(chan >= NUM_MIDI_CHANNELS)
        return;

    printf("Note off:%d \n", note);
    me->writeevent(ntrack, SeqEvent::note(convertdt(dt), chan, note));
}


void MidiFile::parsenoteon(char ntrack, char chan, unsigned int dt)
{
    unsigned char note, vel;
    note = getbyte();
    vel  = getbyte();

    if(chan >= NUM_MIDI_CHANNELS)
        return;

    printf("[dt %d ]  Note on:%d %d\n", dt, note, vel);
    me->writeevent(ntrack, SeqEvent::note(convertdt(dt), chan, note, vel));
}

void MidiFile::parsecontrolchange(char ntrack, char chan, unsigned int dt)
{
    unsigned char control, value;
    control = getbyte();
    value   = getbyte();

    if(chan >= NUM_MIDI_CHANNELS)
        return;

    printf("[dt %d] Control change:%d %d\n", dt, control, value);
    me->writeevent(ntrack, SeqEvent::control(convertdt(dt), chan, control, value));
}

void MidiFile::parsepitchwheel(char ntrack, char chan, unsigned int dt)
{
    unsigned char valhi, vallo;
    vallo = getbyte();
    valhi = getbyte();

    if(chan >= NUM_MIDI_CHANNELS)
        return;

    int value = (int)valhi * 128 + vallo;

    printf("[dt %d] Pitch wheel:%d\n", dt, value);
}

void MidiFile::parsemetaevent(unsigned char mtype, unsigned char mlength)
{
    printf("meta-event type=0x%x  length=%d\n", mtype, mlength);
    skipnbytes(mlength);
}

void MidiFile::add_dt(char ntrack, unsigned int dt)
{
    if(dt == 0)
        return;
    printf("delay-marker track=%d length=%d\n", ntrack, dt);
    me->writeevent(ntrack, SeqEvent::time(convertdt(dt)));
}

void MidiFile::clearmidifile()
{
}

unsigned int MidiFile::convertdt(unsigned int dt)
{
    return dt * 15.0;
}

unsigned char MidiFile::getbyte()
{
    int byte = fgetc(file);
    return byte == EOF ? 0 : byte;
}

unsigned char MidiFile::peekbyte()
{
    int byte = ungetc(fgetc(file), file);
    return byte == EOF ? 0 : byte;
}

unsigned int MidiFile::getint32()
{
    unsigned int result = 0;
    for(int i = 0; i < 4; i++)
        result = result * 256 + getbyte();

    return feof(file) ? 0 : result;
}

unsigned short int MidiFile::getint16()
{
    unsigned short int result = 0;
    for(int i = 0; i < 2; i++)
        result = result * 256 + getbyte();

    return feof(file) ? 0 : result;
}

unsigned int MidiFile::getvarint32()
{
    unsigned long result = 0;
    unsigned char b;

    if((result = getbyte()) & 0x80) {
        result &= 0x7f;
        do {
            b      = getbyte();
            result = (result << 7) + (b & 0x7f);
        } while(b & 0x80);
    }

    return result;
}

void MidiFile::skipnbytes(int n)
{
    fseek(file, n, SEEK_CUR);
}

