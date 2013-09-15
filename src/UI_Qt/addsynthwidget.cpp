#include "addsynthwidget.h"
#include "ui_addsynthwidget.h"

AddSynthWidget::AddSynthWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AddSynthWidget)
{
	ui->setupUi(this);
}

AddSynthWidget::~AddSynthWidget()
{
	delete ui;
}
