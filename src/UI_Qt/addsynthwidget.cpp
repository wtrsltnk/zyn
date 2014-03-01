#include "addsynthwidget.h"
#include "ui_addsynthwidget.h"

#include "ui_funcwidget.h"

AddSynthWidget::AddSynthWidget(QtOscWidget *parent) :
	QtOscWidget(parent),
	ui(new Ui::AddSynthWidget)
{
	ui->setupUi(this);

	// todo: make get_widget4() instead of friend...
	//makeChild(ui->widget->ui->widget_4, "FreqLfo/Pintensity/");

	//makeChild(ui->widget_2, "");
	//makeChild(ui->widget_3, "");
}

AddSynthWidget::~AddSynthWidget()
{
	delete ui;
}

void AddSynthWidget::makeAllChildren(QtOscNode *dest, const char *_loc)
{
	makeChild(ui->widget->ui->widget_4, "FreqLfo/");
}
