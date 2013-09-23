#include "envwidget.h"
#include "ui_envwidget.h"

EnvWidget::EnvWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::EnvWidget)
{
	ui->setupUi(this);
}

EnvWidget::~EnvWidget()
{
	delete ui;
}
