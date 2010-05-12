#ifndef INMGR_H
#define INMGR_H

#include <string>
#include <semaphore.h>
#include "SafeQueue.h"
#include "MidiEvent.h"

class Master;
class MidiIn;
//super simple class to manage the inputs
class InMgr
{
    public:
        InMgr(Master *nmaster);
        ~InMgr();

        void putEvent(MidiEvent ev);

        /**Flush the Midi Queue*/
        void flush();

        bool setSource(std::string name);

        std::string getSource() const;

        friend class EngineMgr;
    private:
        MidiIn *getIn(std::string name);
        SafeQueue<MidiEvent> queue;
        sem_t work;
        MidiIn *current;

        /**the link to the rest of zyn*/
        Master *master;
};

extern InMgr *sysIn;
#endif

