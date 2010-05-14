#ifndef INMGR_H
#define INMGR_H

#include <string>
#include <semaphore.h>
#include "SafeQueue.h"
#include "MidiEvent.h"

//super simple class to manage the inputs
class InMgr
{
    public:
        static InMgr &getInstance();
        ~InMgr();

        void putEvent(MidiEvent ev);

        /**Flush the Midi Queue*/
        void flush();

        bool setSource(std::string name);

        std::string getSource() const;

        friend class EngineMgr;
    private:
        InMgr();
        class MidiIn *getIn(std::string name);
        SafeQueue<MidiEvent> queue;
        sem_t work;
        class MidiIn *current;

        /**the link to the rest of zyn*/
        class Master &master;
};

#endif

