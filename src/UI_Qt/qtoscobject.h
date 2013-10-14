#ifndef QTOSCOBJECT_H
#define QTOSCOBJECT_H

#include <QWidget>
class QtOscObject;
typedef QtOscObject Fl_Osc_Widget; // a bit hackish

#include <UI/Fl_Osc_Interface.h>
//#include <UI/Fl_Osc_Widget.H>
//#include <UI/Fl_Osc_Pane.H>
#include <Misc/MiddleWare.h>

class QString;
class QtOscPane;

/**
 * @brief The QtOscObject class
 * @note This is *not* a QObject class ;)
 */
class QtOscObject
{
public:
	//Callback methods
	virtual void OSC_value(float);
	virtual void OSC_value(bool);
	virtual void OSC_value(int);
	virtual void OSC_value(char);
	virtual void OSC_value(unsigned,void*);
	virtual void OSC_value(const char *);

	//labeled forwarding methods
	virtual void OSC_value(float x, const char *);
	virtual void OSC_value(bool x, const char *);
	virtual void OSC_value(int x, const char *);
	virtual void OSC_value(char x, const char *);
	virtual void OSC_value(unsigned x, void *v, const char *);
	virtual void OSC_value(const char *x, const char *);

	//Raw messages
	virtual void OSC_raw(const char *);

	//Widget methods
	void oscWrite(std::string path, const char *args, ...);
	void oscWrite(std::string path);
	void oscRegister(const char *path);

	//Forces an update of parameters as they have become stale somehow
	virtual void update(void);

	//Smoothly change the base path
	virtual void rebase(std::string new_base);
	void oscMove(std::string new_ext);

	//Base path
	std::string loc;
	//Extension
	std::string ext;
	Fl_Osc_Interface *osc;
protected:
	QtOscPane *fetch_osc_pane(QObject *w);
protected:
	QString	path;
public:
	QtOscObject(QtOscObject* treeBase, const QString& subPath, QObject *self);
	~QtOscObject();
};

//typedef QtOscObject Fl_Osc_Widget; // a bit hackish

#endif // QTOSCOBJECT_H
