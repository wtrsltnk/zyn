#include "filterparamwidget.h"
#include "ui_filterparamwidget.h"

FilterParamWidget::FilterParamWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FilterParamWidget)
{
	ui->setupUi(this);
}

FilterParamWidget::~FilterParamWidget()
{
	delete ui;
}
