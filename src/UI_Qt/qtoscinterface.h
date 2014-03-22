#ifndef QTOSCINTERFACE_H
#define QTOSCINTERFACE_H

#include <QObject>
#include <QDebug> // TODO
#include <src/UI/Fl_Osc_Interface.h> // TODO: not necessary
#include <rtosc/thread-link.h>

class QDial;
class QLabel;
class QSignalMapper;

struct ThreadLinkInterface
{
//	static rtosc::ThreadLink *uRecv;
//	static rtosc::ThreadLink *uSend;

	void writeRaw(const char* data) const {
		printf("SENDING: %s\n", data);
		Fl_Osc_Interface::gToU->raw_write(data); }

    //! this is only used to send data to ourself...
/*    void tryLink(const char* data) const {
        Fl_Osc_Interface::uToG->raw_write(data);
    }*/
};

/*
 * Dial is a node
 * node has signalling struct: non ptr, Qobject
 * Signalling struct has ptr to Fl Osc
 *
 *
 *
 *
 */

template<class T>
class QtWidgetAndPath : public QObject
{
	const QString path;
	const T widget;
	QtWidgetAndPath(const QString& _path, const T& _widget)
	 : path(_path), widget(_widget) {}
};

class QtOscInterface : public QObject
{
	Q_OBJECT
	QSignalMapper *signalMapper;
	QString path;
	void sendMsg(const QString& str, const char *args, ...);
private slots:
	inline void sendMsg(QWidget* obj) {
		qDebug() << "got message!";
		sendMsg((const QWidget*)obj);
	}

	void sendMsg(const QWidget* obj);
	void sendMsg(const QString& str, const char *args, va_list arglist);
	void testSlot() { int i = 0; qDebug() << "test: " << i; }
public:
//	Fl_Osc_Interface* osc; // TODO (public)
	ThreadLinkInterface *osc;


	void requestValue(const char* _str) {
		qDebug() << "requestValue: " << _str;
        osc->requestValue(_str); // TODO...
	}

	void sendMsgFromHere(const QString &str, const char *args, ...);
	inline const QString& getPath() { return path; }
	void fromParent(QtOscInterface *parent);
	void init(QDial *dial, QLabel *label, const char *_loc);
	QtOscInterface(ThreadLinkInterface *_osc, const QString& _path);
};

#endif // QTOSCINTERFACE_H
