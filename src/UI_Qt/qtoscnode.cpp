#include <UI/Fl_Osc_Interface.h> // TODO: necessary?

#include "qtoscnode.h"

/*QtOscNode::QtOscNode(Fl_Osc_Interface *_osc, const QString& _path)
	: osc(new QtOscInterface(_osc)), path(_path)
{
}*/

void QtOscNode::set(Fl_Osc_Interface *_osc, const QString &_path)
{
	osc = new QtOscInterface(_osc, _path);
}

void QtOscNode::makeChild(QtOscNode* dest, const char *_loc) const
{
	//dest->osc->setParent(osc);
	dest->set(osc->osc, osc->getPath() + _loc);
	dest->osc->fromParent(osc);
	dest->makeAllChildren(NULL, NULL);
}

void QtOscNode::makeRoot(QtOscNode* dest, Fl_Osc_Interface *_osc)
{
	dest->set(_osc, "");
	dest->makeAllChildren(NULL, NULL);
}

void QtOscNode::init(QDial *dial, QLabel *label, const char *_loc)
{
	osc->init(dial, label, _loc);
}


QtOscWidget::QtOscWidget(QWidget *parent)
 : QWidget(parent)
{
}


void QtOscNode::setInitialPath(const QString &_path)
{
	osc = new QtOscInterface(NULL, _path);
}

void QtOscNode::makeAllChildren(QtOscNode *dest, const char *_loc)
{
}
