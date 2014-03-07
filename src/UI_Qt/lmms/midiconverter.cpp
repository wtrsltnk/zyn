#include "midiconverter.h"

#include "midi.h"
#include "midi_time.h"

MidiConverter::MidiConverter()
{
}

void MidiConverter::processInEvent(const midiEvent &_me, const midiTime &_time)
{
	(void)_time;
	switch(_me.m_type)
	{
		case MidiNoteOff: emit noteOff(_me.key()); break;
		case MidiNoteOn: emit noteOn(_me.key(), _me.velocity(), 0 /* TODO */); break;
		case MidiKeyPressure: break; // TODO
		default: exit(99);
	}
}

void MidiConverter::processOutEvent(const midiEvent &_me, const midiTime &_time)
{
	// nothing to do
}
