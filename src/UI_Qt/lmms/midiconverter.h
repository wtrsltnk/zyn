#ifndef MIDICONVERTER_H
#define MIDICONVERTER_H

#include <QObject>
#include "MidiEventProcessor.h"
class midiEvent;
class midiTime;

class MidiConverter : public QObject, public MidiEventProcessor
{
	Q_OBJECT
public:
	MidiConverter();

	// to be implemented by inheriting classes
	virtual void processInEvent( const midiEvent & _me,
						const midiTime & _time );
	virtual void processOutEvent( const midiEvent & _me,
						const midiTime & _time );

signals:
	void noteOff(unsigned char note);
	void noteOn(unsigned char note,
		unsigned char velocity,
		int masterkeyshift);
};

#endif // MIDICONVERTER_H
