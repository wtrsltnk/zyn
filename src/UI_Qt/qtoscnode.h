#ifndef QTOSCNODE_H
#define QTOSCNODE_H

#include <QWidget>
class Fl_Osc_Interface;
class QString;
#include <QStringList>
#include "qtoscinterface.h" // TODO

#include "../UI/Fl_Osc_Widget.H"

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
class QtOscNode : public Fl_Osc_Widget
{ public:
	//! note that we can not inherit,
	//! since diamond inheritance from QObject won't work.
	QtOscInterface* osc = nullptr; // TODO!
	QString mainPath;
	QStringList aliasPaths;
private:
	//QtOscNode(Fl_Osc_Interface* _osc, const QString &_path);
	void set(Fl_Osc_Interface *_osc, const QString &_path);
	void setInitialPath(const QString &_path);
	void addAliasPath(const QString &_path);
	void createLink(const QString &_path);
public:
	//void complete(Fl_Osc_Interface _osc, )
	//inline QtOscNode() {}
	virtual void makeAllChildren(QtOscNode* dest, const char* _loc);

	void makeChild(QtOscNode* dest, const char* _loc) const;
	static void makeRoot(QtOscNode* dest, Fl_Osc_Interface *_osc);
	void initDial(QDial *dial, QLabel *label, const char* _loc);
	// right now, mostly a test function
	void initLabel(QDial *dial, QLabel *label, const char *_loc);
	void setMainPath(const QString &_path);
};

// comfort class for widgets
class QtOscWidget : public QWidget, public QtOscNode
{
public:
	QtOscWidget(QWidget *parent = 0);
};



#endif // QTOSCNODE_H
