#include "midiconverter.h"

#include "midi.h"
#include "midi_time.h"

MidiConverter::MidiConverter()
{
}

void MidiConverter::processInEvent(const midiEvent &event, const midiTime &_time)
{
	(void)_time;
	switch(event.m_type)
	{
		case MidiNoteOff: emit noteOff(event.key()); break;
		case MidiNoteOn: emit noteOn(event.key(), event.velocity(), 0 /* TODO */); break;
		case MidiKeyPressure: break; // TODO
		default: exit(99);
	}
}

void MidiConverter::processOutEvent(const midiEvent &, const midiTime &)
{
	// nothing to do
}
