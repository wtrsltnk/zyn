#include "EventClasses.h"

ChangeEvent::ChangeEvent(int nval)
    :Event(Event::ChangeEvent), val(nval)
{}

UpdateEvent::UpdateEvent()
    :Event(Event::UpdateEvent)
{}

NewValueEvent::NewValueEvent(GenControl *control, int value, bool wasChanged)
    :Event(Event::NewValueEvent),
      control(control),
      value(value),
      wasChanged(wasChanged),buf(NULL)
{
    puts("NEWVALUEEVENT: TYPE A");
}

NewValueEvent::~NewValueEvent()
{
    delete[] buf;
}

NewValueEvent::NewValueEvent(GenControl *control, const float *buf)
    :Event(Event::NewValueEvent),
      control(control), value(0),
      wasChanged(false),buf(buf)
{
    puts("NEWVALUEEVENT: TYPE B");
}

RequestValueEvent::RequestValueEvent()
    :Event(Event::RequestValueEvent)
{}

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

