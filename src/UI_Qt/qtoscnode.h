#ifndef QTOSCNODE_H
#define QTOSCNODE_H

class Fl_Osc_Interface;
class QString;
#include "qtoscinterface.h" // TODO

class QtOscNode //: public QtOscInterface
{
	QString path;
	QtOscInterface* osc;
private:
	QtOscNode(Fl_Osc_Interface* _osc, const QString &_path);
public:
	inline QtOscNode() {}
	QtOscNode makeChild(const char* _loc);
	inline static QtOscNode makeRoot(Fl_Osc_Interface* _osc);
	void init(QDial *dial, QLabel *label);
};

#endif // QTOSCNODE_H
