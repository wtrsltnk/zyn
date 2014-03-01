#ifndef QTOSCNODE_H
#define QTOSCNODE_H

#include <QWidget>
class Fl_Osc_Interface;
class QString;
#include "qtoscinterface.h" // TODO

/*class QtOscNode : public QObject
{
	QString path;
	QtOscInterface* osc;
private:
	//QtOscNode(Fl_Osc_Interface* _osc, const QString &_path);
	void set(Fl_Osc_Interface* _osc, const QString &_path);
public:
	//inline QtOscNode() {}
	void makeChild(QtOscNode* dest, const char* _loc) const;
	static void makeRoot(QtOscNode* dest, Fl_Osc_Interface* _osc);
	void init(QDial *dial, QLabel *label, const char* _loc);
};*/

// shall only keep a pointer, nothing else
class QtOscNode
{ public:
	QtOscInterface* osc; // TODO!
private:
	//QtOscNode(Fl_Osc_Interface* _osc, const QString &_path);
	void set(Fl_Osc_Interface* _osc, const QString &_path);
	void setInitialPath(const QString &_path);
public:
	//void complete(Fl_Osc_Interface _osc, )
	//inline QtOscNode() {}
	virtual void makeAllChildren(QtOscNode* dest, const char* _loc);

	void makeChild(QtOscNode* dest, const char* _loc) const;
	static void makeRoot(QtOscNode* dest, Fl_Osc_Interface* _osc);
	void init(QDial *dial, QLabel *label, const char* _loc);

	inline void sendMsgFromHere() {

	}
};

// comfort class for widgets
class QtOscWidget : public QWidget, public QtOscNode
{
public:
	QtOscWidget(QWidget *parent = 0);
};



#endif // QTOSCNODE_H
