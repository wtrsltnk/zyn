#ifndef QTOSCPANE_H
#define QTOSCPANE_H

#include <string> // (TODO: forward)

class QtOscPane
{
public:
	QtOscPane();
	class Fl_Osc_Interface *osc;
	std::string base;
	virtual std::string loc(void) const = 0;
	//! rebase is used when e.g. the voice is changed, but the gui remains
	virtual void rebase(std::string new_base) = 0;
};

#endif // QTOSCPANE_H
