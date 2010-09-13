#include "EventClasses.h"
#include <stdio.h>

ConnEvent::ConnEvent(GenControl *control, const float *buf)
    :Event(Event::ConnEvent), control(control),
    buf(true),data(NULL)
{
    dat.f = buf;
}

ConnEvent::ConnEvent(GenControl *control, int val, void *data)
    :Event(Event::ConnEvent), control(control),
    buf(false),data(data)
{
    dat.i = val;
}

ConnEvent::~ConnEvent()
{
    if(buf)
        delete[] dat.f;
}

NewValueEvent::NewValueEvent(GenControl *control, int value)
    :Event(Event::NewValueEvent),
      control(control),
      value(value),buf(NULL)
{
    puts("NEWVALUEEVENT: TYPE A");
}

NewValueEvent::~NewValueEvent()
{
    delete[] buf;
}

NewValueEvent::NewValueEvent(GenControl *control, const float *buf)
    :Event(Event::NewValueEvent),
      control(control), value(0),buf(buf)
{
    puts("NEWVALUEEVENT: TYPE B");
}

CreateNodeEvent::CreateNodeEvent(unsigned int ntype)
    :Event(Event::CreateNodeEvent), type(ntype)
{}

NewNodeEvent::NewNodeEvent(class Node *nnode)
    :Event(Event::NewNodeEvent), node(nnode)
{}

RemovalEvent::RemovalEvent(Node *node)
    :Event(Event::RemovalEvent), node(node)
{}

MidiEvent::MidiEvent(unsigned char chan, unsigned int type, int par)
    :Event(Event::MidiEvent),
      chan(chan),
      type(type),
      par(par)
{}

