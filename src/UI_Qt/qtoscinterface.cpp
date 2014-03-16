#include <QString>
#include <QSignalMapper>
#include <QLabel>
#include <QDial>

#include <cassert>
#include <QDebug> // TODO
#include <QMessageBox>

#include <rtosc/ports.h>
#include "../Misc/Master.h"

#include "qtoscinterface.h"

rtosc::Port::MetaContainer get_metacontainer(const QString& str)
{
	rtosc::Ports& p = Master::ports;
	const rtosc::Port* port = p.apropos(str.toAscii().data());
	if(!port) {
		QMessageBox::critical(NULL, "Error setting up path!", str);
	}
	return port ?  rtosc::Port::MetaContainer(port->metadata) : rtosc::Port::MetaContainer("");
}

void QtOscInterface::init(QDial *dial, QLabel *label, const char *_loc)
{
	rtosc::Ports& p = Master::ports;
	/*QString loc = path + _loc;
	qDebug()<< loc;
	qDebug() << p.apropos(path.toAscii().data()	)->name;
	qDebug() << p.apropos(loc.toAscii().data())->name;
	qDebug() << p.apropos(loc.toAscii().data())->metadata;*/

	qDebug() << path;

	rtosc::Port::MetaContainer meta = get_metacontainer(path + _loc);
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

	connect(dial, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
	QString subPath = path + _loc;
	signalMapper->setMapping(dial, dial);
//	QObject::connect(ui->dial, SIGNAL(sliderMoved(int), );
	dial->setObjectName(subPath);
	QObject::connect(dial, SIGNAL(valueChanged(int)), this, SLOT(testSlot()));

#ifdef WIDGETS_NEED_UPDATE
	label->update();
	dial->update();
#endif
}

QtOscInterface::QtOscInterface(ThreadLinkInterface *_osc, const QString &_path)
 : signalMapper(new QSignalMapper(this)),
 path(_path)/*,
 osc(_osc)*/
{
	qDebug() << "Initing from path: " << path;
	connect(signalMapper, SIGNAL(mapped(QWidget*)),
		this, SLOT(sendMsg(QWidget*)));
}


void QtOscInterface::sendMsg(const QString &str, const char *args, va_list arglist)
{
	char buffer[1024];


	//puts("writing OSC");
	//printf("Path = '%s'\n", path.c_str());


	// TODO: va version of this function

/*	if(rtosc_vmessage(buffer, 1024, str.toAscii().data(), args, va))
	 osc->writeRaw(buffer);
	else
	 puts("Dangerous Event ommision");*/


	//qDebug() << "Sending msg to rtosc: " << str
	//	<< " [argtypes: ]" << args
	//	<< " [arglist: ](how should I display this?:))";
	//qDebug() << va_arg(arglist, int) << ", " << va_arg(arglist, int) << ", " << va_arg(arglist, int);
	if(
	rtosc_vmessage(buffer, 1024, str.toAscii().data(), args, arglist))
	//rtosc_message(buffer, 1024, "/noteOn", "ccc", 0, 64, 127))
	 osc->writeRaw(buffer);
	else
		puts("Dangerous Event ommision");
}

void QtOscInterface::sendMsg(const QString &str, const char *args, ...)
{
	va_list va;
	va_start(va, args);
	sendMsg(str, args, va);
	va_end(va);
}

void QtOscInterface::sendMsgFromHere(const QString &str,
	const char *args, ...)
{
	va_list l;
	va_start(l, args);
	sendMsg(path + str, args, l);
	va_end(l);
}


void QtOscInterface::fromParent(QtOscInterface* _parent)
{ // TODO: remove me
//	setParent(_parent);

}


void QtOscInterface::sendMsg(const QWidget *obj)
{
	//double value;
	// maybe this is not the best way, but it is the only way...
	// templates might have been cooler, but Qt will forbid that.
	if (const QDial *dial = qobject_cast<const QDial*>(obj)) {
		//value = dial->value();

		sendMsg(obj->objectName(), "c", (char)dial->value());

	} /*else if (QPushButton *button = qobject_cast<QPushButton *>(obj)) {
		button->setText(tr("Pong!"));
	}*/
	else
		assert(false);
	//sendMsg(obj->objectName(),QString::number(value).toAscii().data());
}
