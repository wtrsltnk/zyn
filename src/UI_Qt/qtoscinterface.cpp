#include <QString>
#include <QSignalMapper>
#include <QDial>

#include <cassert>
#include <QDebug> // TODO

#include "qtoscinterface.h"

void QtOscInterface::init(QDial *dial)
{
	connect(dial, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
	signalMapper->setMapping(dial, dial);
	QObject::connect(dial, SIGNAL(valueChanged(int)), this, SLOT(testSlot()));

#ifdef WIDGETS_NEED_UPDATE
	label->update();
	dial->update();
#endif
}

QtOscInterface::QtOscInterface(Fl_Osc_Interface *_osc)
 : signalMapper(new QSignalMapper(this)),
 //mainPath(_path),
 osc(_osc)
{
//	qDebug() << "Initing from path: " << mainPath;
	connect(signalMapper, SIGNAL(mapped(QWidget*)),
		this, SLOT(sendMsg(QWidget*)));
}


void QtOscInterface::sendMsg(const QString &str, const char *args, va_list arglist)
{
	char buffer[1024];

	// TODO: va version of this function

/*	if(rtosc_vmessage(buffer, 1024, str.toAscii().data(), args, va))
	 osc->writeRaw(buffer);
	else
	 puts("Dangerous Event ommision");*/


	qDebug() << "Sending msg to rtosc: " << str
		<< " [argtypes: ]" << args
		<< " [arglist: ](how should I display this?:))";
	//qDebug() << va_arg(arglist, int) << ", " << va_arg(arglist, int) << ", " << va_arg(arglist, int);
	if(
	rtosc_vmessage(buffer, 1024, str.toAscii().data(), args, arglist))
	 osc->writeRawUi(buffer);
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

void QtOscInterface::sendMsg(const QWidget *obj)
{
	// TODO: use one double var for all?
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
