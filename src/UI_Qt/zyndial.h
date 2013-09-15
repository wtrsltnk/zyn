#ifndef ZYNDIAL_H
#define ZYNDIAL_H

#include <QWidget>

// TODO: declare fwd?
#include <rtosc/ports.h>

namespace Ui {
	class ZynDial;
}

class ZynDial : public QWidget
{
	Q_OBJECT
	
	rtosc::Port::MetaContainer meta;
public:
	explicit ZynDial(QWidget *parent = 0);
	~ZynDial();
	
private:
	Ui::ZynDial *ui;
};

#endif // ZYNDIAL_H
