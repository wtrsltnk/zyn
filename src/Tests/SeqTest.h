#include <iostream>
#include <cstdio>
#include "../Seq/Sequencer.h"
using namespace std;

bool some_bool(bool *array)
{
    bool ans = false;
    for(int i = 0; i < NUM_MIDI_TRACKS; i++)
        ans |= array[i];
    return ans;
}

class SeqTest:public CxxTest::TestSuite
{
    public:
        void testLoad()
        {
            string location = string(SOURCE_DIR) + string("/Tests/test.midi");
            Sequencer seq;
            seq.importmidifile(location.c_str());

            seq.startplay();

            bool tracks[NUM_MIDI_TRACKS];
            for(int i = 0; i < NUM_MIDI_TRACKS; i++)
                tracks[i] = true;
            
            while(some_bool(tracks)) {
                int again;
                for(int ntrack = 0; ntrack < NUM_MIDI_TRACKS; ntrack++) {
                    if(!tracks[ntrack])
                        continue;
                    MidiEvent ev;
                    again = seq.getevent(ntrack, ev);

                    if(ev.type == -1)
                        tracks[ntrack] = false;
                    if(ev.type > 0) {
                        if(ev.type == 1) { //note_on or note_off
                            if(ev.value != 0)
                                printf("NoteOn on %d for %d at %d\n", ev.channel, ev.num, ev.value);
                            else
                                printf("NoteOff on %d for %d\n", ev.channel, ev.num);
                        }
                        if(ev.type == 255)
                            printf("Timing Info\n");
                    }
                }
                usleep(1000);
            }
//exit_hop:;
        }
};
