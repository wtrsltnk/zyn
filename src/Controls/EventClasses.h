#ifndef EVENTCLASSES_H
#define EVENTCLASSES_H

#include "Event.h"
#include "GenControl.h"

/**Connection event*/
struct ConnEvent:public Event
{
    ConnEvent(GenControl *control, const float *buf);
    ConnEvent(GenControl *control, int val, void *data=NULL);
    ~ConnEvent();

    GenControl *control;

    bool buf;
    union{
        int i;
        const float *f;
    } dat;

    //Currently used for Selectors, but could be used for something else
    const void *data;
};

/** Emitted from the node when its value has changed **/
class NewValueEvent:public Event
{
    public:
        NewValueEvent(GenControl *control, const float *buf);
        NewValueEvent(GenControl *control, int value);
        ~NewValueEvent();
        const GenControl *control;
        int value;
        const float *buf;
};

/**Event for Child creation*/
class CreateNodeEvent:public Event
{
    public:
        //I recommend that an enum gets put into ntype
        //ie. an Instrument could have an enum of
        //ADsynth=0,SUBsynth=1,PADsynth=2
        //This means that an int with more meaning is passed here
        CreateNodeEvent(unsigned int ntype);
        unsigned int getType() const {return type;}
    private:
        const unsigned int type;
};

/* Notification that a node is about to be removed */
class RemovalEvent:public Event
{
    public:
        RemovalEvent(Node *node);
        Node *getNode() const { return node; }
    private:
        Node *node;
};

class OptionsChangedEvent:public Event
{
    public:
        OptionsChangedEvent():Event(Event::OptionsChangedEvent) {}
};

class MidiEvent:public Event
{
    public:
        MidiEvent(unsigned char chan, unsigned int type, int par);
        int chan, type, par;
};

/**Event for notification of the creation of a new node*/
class NewNodeEvent:public Event
{
    public:
        NewNodeEvent(class Node *nnode);
        class Node *getNode() {return node;}
    private:
        class Node * node;
};
#endif // EVENTCLASSES_H

