#ifndef QTOSCINTERFACE_H
#define QTOSCINTERFACE_H

#include <QObject>
#include <QDebug> // TODO
#include <UI/Fl_Osc_Interface.h> // TODO: not necessary

class QDial;
class QLabel;
class QSignalMapper;

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
	Fl_Osc_Interface* osc; // TODO (public)

	void requestValue(const char* _str) {
		qDebug() << "requestValue: " << _str;
		osc->requestValue(_str);
		}
	void sendMsgFromHere(const QString &str, const char *args, ...);
	inline const QString& getPath() { return path; }
	void fromParent(QtOscInterface *parent);
	void init(QDial *dial, QLabel *label, const char *_loc);
	QtOscInterface(Fl_Osc_Interface* _osc, const QString& _path);
};

#endif // QTOSCINTERFACE_H
