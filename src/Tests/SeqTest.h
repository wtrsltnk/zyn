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
        bool some(bool *array)
        {
            bool ans = false;
            for(int i = 0; i < NUM_MIDI_TRACKS; i++)
                ans |= array[i];
            return ans;
        }
        void testLoad()
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
                        cout << ev << endl;
                }
                usleep(1000);
            }
        }
};
