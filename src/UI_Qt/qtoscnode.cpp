#include <UI/Fl_Osc_Interface.h> // TODO: necessary?

#include <QLabel>
#include <QDial>
#include <QMessageBox>
#include <QDebug>

#include "qtoscnode.h"
#include "qtoscinterface.h"
#include <rtosc/ports.h>
#include "../Misc/Master.h"

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
		qDebug() << "Appending alias: " << _path.toAscii().data();
	}
	else
	{
		osc = new QtOscInterface(_osc);
		setMainPath(_path);
	}
}

void QtOscNode::createLink(const QString& _path)
{
	osc->osc->createLink(_path.toAscii().data(), this);
}

void QtOscNode::makeChild(QtOscNode* dest, const char *_loc) const
{
	qDebug() << "Making child:" << (mainPath + _loc).toAscii().data();

	dest->set(osc->osc, mainPath + _loc);
	if(dest != this) {
		qDebug() << dest << ", " << this;
		dest->makeAllChildren();
	}
}

void QtOscNode::makeRoot(QtOscNode* dest, Fl_Osc_Interface *_osc)
{
	dest->set(_osc, "/");
	dest->makeAllChildren();
}

void QtOscNode::initDial(QDial *dial, QLabel *label, const char *_loc)
{
	initLabel(dial, label, _loc);
}


QtOscWidget::QtOscWidget(QWidget *parent)
 : QWidget(parent)
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

	osc->init(dial);
}

