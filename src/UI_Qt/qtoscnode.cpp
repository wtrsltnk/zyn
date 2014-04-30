#include <UI/Fl_Osc_Interface.h> // TODO: necessary?

#include <QLabel>
#include <QDial>
#include <QMessageBox>

#include "qtoscnode.h"
#include <iostream> // TODO
#include <rtosc/ports.h>
#include "../Misc/Master.h"


/*QtOscNode::QtOscNode(Fl_Osc_Interface *_osc, const QString& _path)
	: osc(new QtOscInterface(_osc)), path(_path)
{
}*/

void QtOscNode::addAliasPath(const QString& _path)
{
	aliasPaths.append(_path);
	createLink(_path);
}

void QtOscNode::setMainPath(const QString& _path)
{
	mainPath = _path;
	createLink(_path);
}

void QtOscNode::set(Fl_Osc_Interface *_osc, const QString &_path)
{
	// TODO: better: don't depend on the order the user calls makeChild()?
	if(osc) {
		addAliasPath(_path);
		std::cout << "Appending alias: " << _path.toAscii().data() << std::endl;
	}
	else
	{
		osc = new QtOscInterface(_osc, _path);
		setMainPath(_path);
	}
}

void QtOscNode::createLink(const QString& _path)
{
	osc->osc->createLink(_path.toAscii().data(), this);
}

void QtOscNode::makeChild(QtOscNode* dest, const char *_loc) const
{
	std::cout << "Making child:" << (mainPath + _loc).toAscii().data() << std::endl;

	//dest->osc->setParent(osc);
	dest->set(osc->osc, mainPath + _loc);
//	dest->osc->fromParent(osc);
	if(dest != this) {
		std::cout << dest << ", " << this << std::endl;
		dest->makeAllChildren(NULL, NULL);
	}
}

void QtOscNode::makeRoot(QtOscNode* dest, Fl_Osc_Interface *_osc)
{
	dest->set(_osc, "/");
	dest->makeAllChildren(NULL, NULL);
}

void QtOscNode::initDial(QDial *dial, QLabel *label, const char *_loc)
{
	initLabel(dial, label, _loc);
}


QtOscWidget::QtOscWidget(QWidget *parent)
 : QWidget(parent)
{
}


void QtOscNode::setInitialPath(const QString &_path)
{
	osc = new QtOscInterface(NULL, _path);
}

// TODO: set this virtual function = 0 ?
void QtOscNode::makeAllChildren(QtOscNode *dest, const char *_loc)
{
}

rtosc::Port::MetaContainer get_metacontainer(const QString& str)
{
	rtosc::Ports& p = Master::ports;
	const rtosc::Port* port = p.apropos(str.toAscii().data());
	if(!port) {
		QMessageBox::critical(NULL, "Error setting up path!", str);
	}
	return port ?  rtosc::Port::MetaContainer(port->metadata) : rtosc::Port::MetaContainer("");
}

void QtOscNode::initLabel(QDial *dial, QLabel *label, const char *_loc)
{
	rtosc::Ports& p = Master::ports;
	/*QString loc = path + _loc;
	qDebug()<< loc;
	qDebug() << p.apropos(path.toAscii().data()	)->name;
	qDebug() << p.apropos(loc.toAscii().data())->name;
	qDebug() << p.apropos(loc.toAscii().data())->metadata;*/

//	qDebug() << mainPath;

	rtosc::Port::MetaContainer meta = get_metacontainer(mainPath + _loc);
	for(const auto x : meta)
	 qDebug() << " ... contains property " << x.title;

	//dial->setMinimum(atof(meta["min"]));
	//dial->setMaximum(atof(meta["max"]));
	assert(meta["min"] != NULL);
	qDebug() << "min" << meta["min"];
	qDebug() << "max" << meta["max"];
	qDebug() << "doc" << meta["documentation"];

	QString labelText = meta["documentation"];
	labelText += "\n[";
	labelText += meta["min"];
	labelText += "..";
	labelText += meta["max"];
	labelText += "]";
	label->setText(labelText);

	QString subPath = mainPath + _loc;
	dial->setObjectName(subPath);
}

