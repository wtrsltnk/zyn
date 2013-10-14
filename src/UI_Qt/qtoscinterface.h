#ifndef QTOSCINTERFACE_H
#define QTOSCINTERFACE_H

#include <QObject>
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


class QtOscInterface : public QObject
{
	Q_OBJECT
	QSignalMapper *signalMapper;
	Fl_Osc_Interface* osc;
	QString path;
private slots:
	void sendMsg(const QString& str);
public:
	void init(QDial *dial, QLabel *label, const char *_loc);
	QtOscInterface(Fl_Osc_Interface* _osc, const QString& _path);
};

#endif // QTOSCINTERFACE_H
