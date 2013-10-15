#include "addsynthwidget.h"
#include "ui_addsynthwidget.h"

AddSynthWidget::AddSynthWidget(QtOscObject& parent) :
	QtOscNode(parent),
	ui(new Ui::AddSynthWidget)
{
	ui->setupUi(this);
}

AddSynthWidget::~AddSynthWidget()
{
	delete ui;
}
