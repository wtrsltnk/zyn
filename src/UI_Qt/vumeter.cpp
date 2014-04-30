// TODO: license (everywhere)
#include <cstdio> // TODO
#include <rtosc/rtosc.h>
#include "../../src/Misc/Master.h"
#include "../../src/Misc/Util.h"

#include "vumeter.h"
#include "ui_vumeter.h"

constexpr int MIN_DB = -48;

float VuMeter::limit(float x)
{
	return ::limit(x, 0.0f, 1.0f);
}

void VuMeter::OSC_raw(const char * m)
{
	rtosc_arg_t a0 = rtosc_argument(m,0);
	rtosc_arg_t a1 = rtosc_argument(m,1);
	if(a0.b.len == sizeof(vuData) &&
		a1.b.len == sizeof(float)*NUM_MIDI_PARTS) {

		vuData* vu_data = (vuData*)a0.b.data;
		// TODO: is this wanted? rmspeak or outpeak?

		int rmsdbr = 100.0f * limit((MIN_DB-rap2dB(vu_data->rmspeakr))/MIN_DB);
		int rmsdbl = 100.0f * limit((MIN_DB-rap2dB(vu_data->rmspeakl))/MIN_DB);

		ui->progressBar->setValue(rmsdbr);
		ui->progressBar_2->setValue(rmsdbl);

		// TODO: what is left and right?
		// TODO: denote this as QLabel?
	}
}

VuMeter::VuMeter(QWidget *parent) :
	QtOscWidget(parent),
	ui(new Ui::VuMeter)
{
	ui->setupUi(this);
	connect(&requestTimer, SIGNAL(timeout()),
		this, SLOT(request_update()));
}

VuMeter::~VuMeter()
{
	delete ui;
}
