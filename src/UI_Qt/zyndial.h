#ifndef ZYNDIAL_H
#define ZYNDIAL_H

#include <QWidget>
#include <rtosc/ports.h>

#ifdef EMBEDDED_IN_LMMS
#include "knob.h"
#else
namespace Ui {
	class ZynDial;
}
#endif

class ZynDial : public QWidget
{
	Q_OBJECT
	
	rtosc::Port::MetaContainer meta;
public:
	explicit ZynDial(QWidget *parent = 0);
	~ZynDial();
	
private:
#ifdef EMBEDDED_IN_LMMS
	knob ui;
#else
	Ui::ZynDial *ui;
#endif
};

#endif // ZYNDIAL_H
