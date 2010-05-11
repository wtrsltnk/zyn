#include <iostream>
#include <cstdio>
#include "../Seq/Sequencer.h"
using namespace std;

bool some(bool *array)
{
    bool ans = false;
    for(int i = 0; i < NUM_MIDI_TRACKS; i++)
        ans |= array[i];
    return ans;
}

class SeqTest:public CxxTest::TestSuite
{
    public:
        int printEvent(Sequencer &seq)
        {
            int track = 1, midich = -1, type = -1, par1 = -1, par2 = -1, result = 0;
            result = seq.getevent(track, midich, type, par1, par2);
            if(!result)
                printf("Event: %d %d %d %d\n", midich, type, par1, par2);
            return result;
        }

        void testLoad()
        {
            string location = string(SOURCE_DIR) + string("/Tests/test.midi");
            Sequencer seq;
            seq.importmidifile(location.c_str());

            seq.startplay();
            //while(printEvent(seq)>-10);
            bool tracks[NUM_MIDI_TRACKS];
            for(int i = 0; i < NUM_MIDI_TRACKS; i++)
                tracks[i] = true;
            
            while(some(tracks)) {
                int type, par1, par2, again, midichan;
                for(int ntrack = 0; ntrack < NUM_MIDI_TRACKS; ntrack++) {
                    if(!tracks[ntrack])
                        continue;
                    again = seq.getevent(ntrack, midichan, type, par1, par2);
                    if(type == -1)
                        tracks[ntrack] = false;
                    if(type > 0) {
                        if(type == 1) { //note_on or note_off
                            if(par2 != 0)
                                printf("NoteOn on %d for %d at %d\n", midichan, par1, par2);
                            else
                                printf("NoteOff on %d for %d\n", midichan, par1);
                        }
                        if(type == 255)
                            printf("Timing Info\n");
                    }
                }
                usleep(1000);
            }
//exit_hop:;
        }
};
