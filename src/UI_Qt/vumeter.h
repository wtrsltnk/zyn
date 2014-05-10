#ifndef VUMETER_H
#define VUMETER_H

#include <QTimer>
#include "qtoscnode.h"
#include "qtoscinterface.h"

namespace Ui {
	class VuMeter;
}

class VuMeter : public QtOscWidget
{
	Q_OBJECT
	QTimer requestTimer;
	const int requestInterval = 1000/60; // 60 fps
private slots:
	void request_update() { osc->requestValue("/get-vu"); }
public:
	explicit VuMeter(QWidget *parent = 0);
	~VuMeter();
	void makeAllChildren() {
		requestTimer.start(requestInterval);
	}
	
private:
	static float limit(float x);
	Ui::VuMeter *ui;
	void OSC_raw(const char *m);
};

#endif // VUMETER_H
