#ifndef QTOSCNODE_H
#define QTOSCNODE_H

#include <QWidget>
#include <QStringList>

class Fl_Osc_Interface;
class QString;
class QtOscInterface;
class QDial;
class QLabel;

#include "../UI/Fl_Osc_Widget.H"

//! keeps the QtOscInterface and all paths
//! receives OSC messages directly via Fl_Osc_Widget
class QtOscNode : public Fl_Osc_Widget
{
	//! note that we can not inherit,
	//! since diamond inheritance from QObject won't work.
protected:
	QtOscInterface* osc = nullptr; // TODO!
	QString mainPath;
	QStringList aliasPaths;
private:
	void set(Fl_Osc_Interface *_osc, const QString &_path);
	void addAliasPath(const QString &_path);
	void createLink(const QString &_path);
protected:
	virtual void makeAllChildren() = 0;

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
