#include "lfoenvwidget.h"
#include "ui_lfoenvwidget.h"

// rParamF(foobar, rMap(min, -1.0), rMap(max, 20.0), "some port")

LfoEnvWidget::LfoEnvWidget(QWidget *parent) :
	QtOscWidget(parent),
	ui(new Ui::LfoEnvWidget)
{
	ui->setupUi(this);
}

LfoEnvWidget::~LfoEnvWidget()
{
	delete ui;
}

void LfoEnvWidget::makeAllChildren()
{
	initDial(ui->dial, ui->label, "Pintensity"); // TODO: is this correct here? or makeChild()?
}
