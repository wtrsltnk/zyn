#include <cassert>
#include <QObject>
#include <QString>
#include "qtoscobject.h"
#include "qtoscpane.h"

class Fl_Widget : public QObject
{
};

QtOscObject::QtOscObject(QtOscObject* treeBase,
	const QString& subPath, QObject* self) /*Fl_Widget* self*/
{
/*	assert(fetch_osc_pane(self));
	if(auto *pane = fetch_osc_pane(self)) {
		osc = pane->osc;
		loc = pane->loc();
	}
	assert(osc);*/ // (TODO)
	path = treeBase ? (treeBase->path + subPath)
		: subPath;
}

#include <rtosc/rtosc.h>

/*QtOscObject::QtOscObject(void) //Deprecated
:loc(), osc(NULL)
{}*/

QtOscObject::~QtOscObject()
{
	if(osc)
	 osc->removeLink(this);
}

void QtOscObject::OSC_value(float) {}
void QtOscObject::OSC_value(bool) {}
void QtOscObject::OSC_value(int) {}
void QtOscObject::OSC_value(char) {}
void QtOscObject::OSC_value(unsigned,void*) {}
void QtOscObject::OSC_value(const char *) {}

//labeled forwarding methods
void QtOscObject::OSC_value(float x, const char *) {OSC_value(x);}
void QtOscObject::OSC_value(bool x, const char *) {OSC_value(x);}
void QtOscObject::OSC_value(int x, const char *) {OSC_value(x);}
void QtOscObject::OSC_value(char x, const char *) {OSC_value(x);}
void QtOscObject::OSC_value(unsigned x, void *v, const char *) {OSC_value(x,v);}
void QtOscObject::OSC_value(const char *x, const char *) {OSC_value(x);}

void QtOscObject::OSC_raw(const char *)
{}


void QtOscObject::oscWrite(std::string path, const char *args, ...)
{
	char buffer[1024];
	//puts("writing OSC");
	//printf("Path = '%s'\n", path.c_str());

	va_list va;
	va_start(va, args);

	if(rtosc_vmessage(buffer, 1024, (loc+path).c_str(), args, va))
	osc->writeRaw(buffer);
	else
	puts("Dangerous Event ommision");
	////Try to pretty print basic events
	//if(!strcmp(args, "c") || !strcmp(args, "i"))
	//    printf("Args = ['%d']\n", rtosc_argument(buffer, 0).i);
	//if(!strcmp(args, "f"))
	//    printf("Args = ['%f']\n", rtosc_argument(buffer, 0).f);
	//if(!strcmp(args, "T"))
	//    printf("Args = [True]\n");
	//if(!strcmp(args, "F"))
	//    printf("Args = [False]\n");
}

void QtOscObject::oscWrite(std::string path)
{
	osc->requestValue(loc+path);
}

void QtOscObject::oscRegister(const char *path)
{
	osc->createLink(loc+path, this);
	osc->requestValue(loc+path);
}

void QtOscObject::update(void)
{
	osc->requestValue(loc+ext);
}

QtOscPane *QtOscObject::fetch_osc_pane(QObject *w)
{
	if(!w)
	 return NULL;

	QtOscPane *pane = dynamic_cast<QtOscPane*>(w->parent());
	if(pane)
	 return pane;
	return fetch_osc_pane(w->parent());
}

void QtOscObject::rebase(std::string new_base)
{
	osc->renameLink(loc+ext, new_base+ext, this);
	loc = new_base;
	osc->requestValue(loc+ext);
}

void QtOscObject::oscMove(std::string new_ext)
{
	osc->renameLink(loc+ext, loc+new_ext, this);
	ext = new_ext;
	osc->requestValue(loc+ext);
}


