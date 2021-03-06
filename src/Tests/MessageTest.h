/*
  ZynAddSubFX - a software synthesizer

  PluginTest.h - CxxTest for embedding zyn
  Copyright (C) 2013-2013 Mark McCurry
  Authors: Mark McCurry

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
#include <cxxtest/TestSuite.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <rtosc/thread-link.h>
#include <unistd.h>
#include "../Misc/MiddleWare.h"
#include "../Misc/Master.h"
#include "../Misc/Part.h"
#include "../Misc/PresetExtractor.h"
#include "../Misc/PresetExtractor.cpp"
#include "../Misc/Util.h"
#include "../globals.h"
#include "../UI/NSM.H"
class NSM_Client *nsm = 0;
MiddleWare *middleware = 0;

using namespace std;

char *instance_name=(char*)"";

#define NUM_MIDDLEWARE 3

class MessageTest:public CxxTest::TestSuite
{
    public:
        Config config;
        void setUp() {
            synth    = new SYNTH_T;
            mw       = new MiddleWare(std::move(*synth), &config);
            ms       = mw->spawnMaster();
            realtime = NULL;
        }

        void tearDown() {
            delete mw;
            delete synth;
        }

        void testKitEnable(void)
        {
            const char *msg = NULL;
            mw->transmitMsg("/part0/kit0/Psubenabled", "T");
            TS_ASSERT(ms->uToB->hasNext());
            msg = ms->uToB->read();
            TS_ASSERT_EQUALS(string("/part0/kit0/subpars-data"), msg);
            TS_ASSERT(ms->uToB->hasNext());
            msg = ms->uToB->read();
            TS_ASSERT_EQUALS(string("/part0/kit0/Psubenabled"), msg);
        }

        void testBankCapture(void)
        {
            mw->transmitMsg("/bank/slot23", "");
            TS_ASSERT(!ms->uToB->hasNext());
            mw->transmitMsg("/bank/fake", "");
            TS_ASSERT(ms->uToB->hasNext());
            const char *msg = ms->uToB->read();
            TS_ASSERT_EQUALS(string("/bank/fake"), msg);
        }

        void testOscCopyPaste(void)
        {
            //Enable pad synth
            mw->transmitMsg("/part0/kit0/Ppadenabled", "T");

            TS_ASSERT(ms->uToB->hasNext());
            ms->applyOscEvent(ms->uToB->read());
            TS_ASSERT(ms->uToB->hasNext());
            ms->applyOscEvent(ms->uToB->read());
            TS_ASSERT(!ms->uToB->hasNext());

	    auto &osc_src = *ms->part[0]->kit[0].adpars->VoicePar[0].FMSmp;
            auto &osc_dst = *ms->part[0]->kit[0].padpars->oscilgen;
            auto &osc_oth = *ms->part[0]->kit[0].adpars->VoicePar[1].OscilSmp;

	    TS_ASSERT_EQUALS(osc_src.Pbasefuncpar, 64);
            osc_src.Pbasefuncpar = 32;
	    TS_ASSERT_EQUALS(osc_src.Pbasefuncpar, 32);

            //Copy From ADsynth modulator
            printf("====Copy From ADsynth modulator\n");
	    start_realtime();
            mw->transmitMsg("/presets/copy", "s", "/part0/kit0/adpars/VoicePar0/FMSmp/");

            TS_ASSERT_EQUALS(osc_dst.Pbasefuncpar, 64);
            TS_ASSERT_EQUALS(osc_oth.Pbasefuncpar, 64);

            //Paste to PADsynth
            printf("====Paste to PADsynth\n");
            mw->transmitMsg("/presets/paste", "s", "/part0/kit0/padpars/oscilgen/");

	    printf("====Paste to ADsynth\n");
            mw->transmitMsg("/presets/paste", "s", "/part0/kit0/adpars/VoicePar1/OscilSmp/");

	    stop_realtime();
            TS_ASSERT_EQUALS(osc_dst.Pbasefuncpar, 32);
            TS_ASSERT_EQUALS(osc_oth.Pbasefuncpar, 32);
        }


        void start_realtime(void)
        {
            do_exit = false;
            realtime = new std::thread([this](){
                    int tries = 0;
                    while(tries < 10000) {
                        if(!ms->uToB->hasNext()) {
                            if(do_exit)
                                break;

                            usleep(500);
                            continue;
                        }
                        const char *msg = ms->uToB->read();
                        printf("RT: handling <%s>\n", msg);
                        ms->applyOscEvent(msg);
                    }});
        }

        void stop_realtime(void)
        {
            do_exit = true;
            realtime->join();
            delete realtime;
            realtime = NULL;
        }

        void run_realtime(void)
        {
            start_realtime();
            stop_realtime();
        }

        void testMidiLearn(void)
        {
            mw->transmitMsg("/learn", "s", "/Pvolume");
            mw->transmitMsg("/virtual_midi_cc", "iii", 0, 23, 108);
            TS_ASSERT_EQUALS(ms->Pvolume, 80);

            //Perform a learning operation

            run_realtime(); //1. runs learning and identifies a CC to bind
            mw->tick();     //2. produces new binding table
            run_realtime(); //3. applies new binding table


            //Verify that the learning actually worked
            mw->transmitMsg("/virtual_midi_cc", "iii", 0, 23, 13);
            run_realtime();
            TS_ASSERT_EQUALS(ms->Pvolume, 13);

            mw->transmitMsg("/virtual_midi_cc", "iii", 0, 23, 2);
            run_realtime();
            TS_ASSERT_EQUALS(ms->Pvolume, 2);

            mw->transmitMsg("/virtual_midi_cc", "iii", 0, 23, 0);
            run_realtime();
            TS_ASSERT_EQUALS(ms->Pvolume, 0);

            mw->transmitMsg("/virtual_midi_cc", "iii", 0, 23, 127);
            run_realtime();
            TS_ASSERT_EQUALS(ms->Pvolume, 127);
        }

        void testMidiLearnSave(void)
        {
            mw->transmitMsg("/learn", "s", "/Pvolume");
            mw->transmitMsg("/virtual_midi_cc", "iii", 0, 23, 108);

            //Perform a learning operation

            run_realtime(); //1. runs learning and identifies a CC to bind
            mw->tick();     //2. produces new binding table
            run_realtime(); //3. applies new binding table

            mw->transmitMsg("/save_xlz", "s", "test-midi-learn.xlz");
            mw->transmitMsg("/load_xlz", "s", "test-midi-learn.xlz");
        }

        void testLfoPaste(void)
        {
            start_realtime();
            ms->part[0]->kit[0].adpars->GlobalPar.FreqLfo->Pfreqrand = 32;
            TS_ASSERT_EQUALS(ms->part[0]->kit[0].adpars->GlobalPar.FreqLfo->Pfreqrand, 32);

            //Copy
            mw->transmitMsg("/presets/copy", "s", "/part0/kit0/adpars/GlobalPar/FreqLfo/");

            ms->part[0]->kit[0].adpars->GlobalPar.FreqLfo->Pfreqrand = 99;
            TS_ASSERT_EQUALS(ms->part[0]->kit[0].adpars->GlobalPar.FreqLfo->Pfreqrand, 99);

            //Paste
            mw->transmitMsg("/presets/paste", "s", "/part0/kit0/adpars/GlobalPar/FreqLfo/");
            stop_realtime();

            TS_ASSERT_EQUALS(ms->part[0]->kit[0].adpars->GlobalPar.FreqLfo->Pfreqrand, 32);
        }

        void testPadPaste(void)
        {
            mw->transmitMsg("/part0/kit0/Ppadenabled", "T");
            run_realtime();

            start_realtime();

            auto &field1 = ms->part[0]->kit[0].padpars->PVolume;
            auto &field2 = ms->part[0]->kit[0].padpars->oscilgen->Pfilterpar1;
            field1 = 32;
            TS_ASSERT_EQUALS(field1, 32);
            field2 = 35;
            TS_ASSERT_EQUALS(field2, 35);

            //Copy
            mw->transmitMsg("/presets/copy", "s", "/part0/kit0/padpars/");

            field1 = 99;
            TS_ASSERT_EQUALS(field1, 99);
            field2 = 95;
            TS_ASSERT_EQUALS(field2, 95);

            //Paste
            mw->transmitMsg("/presets/paste", "s", "/part0/kit0/padpars/");
            stop_realtime();

            TS_ASSERT_EQUALS(field1, 32);
            TS_ASSERT_EQUALS(field2, 35);
        }


    private:
        SYNTH_T     *synth;
        MiddleWare  *mw;
        Master      *ms;
        std::thread *realtime;
        bool         do_exit;
};
