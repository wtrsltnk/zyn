#include <UI/Fl_Osc_Interface.h> // TODO: necessary?

#include "qtoscnode.h"

QtOscNode::QtOscNode(Fl_Osc_Interface *_osc, const QString& _path)
	: osc(new QtOscInterface(_osc)), path(_path)
{
}

QtOscNode QtOscNode::makeChild(const char *_loc)
{
	return QtOscNode(osc, path + _loc);
}

QtOscNode QtOscNode::makeRoot(Fl_Osc_Interface *_osc)
{
	return QtOscNode(_osc, "/");
}

void QtOscNode::init(QDial *dial, QLabel *label)
{
	osc->init(dial, label);
}
