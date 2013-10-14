#include <QString>
#include <QSignalMapper>
#include <QLabel>
#include <QDial>

#include <cassert>
#include <QDebug> // TODO

#include <rtosc/ports.h>
#include "../Misc/Master.h"

#include "qtoscinterface.h"

void QtOscInterface::init(QDial *dial, QLabel *label)
{
	rtosc::Ports& p = Master::ports;
	/*QString loc = path + _loc;
	qDebug()<< loc;
	qDebug() << p.apropos(path.toAscii().data()	)->name;
	qDebug() << p.apropos(loc.toAscii().data())->name;
	qDebug() << p.apropos(loc.toAscii().data())->metadata;*/


	rtosc::Port::MetaContainer meta(p.apropos(path.toAscii().data())->metadata);
	for(const auto x : meta)
	 qDebug() << " ... contains property " << x.title;

	//dial->setMinimum(atof(meta["min"]));
	//dial->setMaximum(atof(meta["max"]));
	assert(meta["min"] != NULL);
	qDebug() << "min" << meta["min"];
	qDebug() << "max" << meta["max"];

	label->setText(meta["documentation"]);

	connect(dial, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
	signalMapper->setMapping(dial, path); // TODO: loc is local, a problem?
}

QtOscInterface::QtOscInterface(Fl_Osc_Interface *_osc, const QString &_path)
 : signalMapper(new QSignalMapper(this)),
 osc(_osc),
 path(_path)
{
	connect(signalMapper, SIGNAL(mapped(QString)),
	    this, SLOT(sendMsg(QString)));
}


void QtOscInterface::sendMsg(const QString &str)
{
	char buffer[1024];
	//puts("writing OSC");
	//printf("Path = '%s'\n", path.c_str());

	va_list va;
	va_start(va, args);

	if(rtosc_vmessage(buffer, 1024, (loc+path).c_str(), args, va))
	 osc->writeRaw(buffer);
	else
	 puts("Dangerous Event ommision");
}
