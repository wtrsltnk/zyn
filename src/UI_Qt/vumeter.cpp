#include "vumeter.h"
#include "ui_vumeter.h"

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
