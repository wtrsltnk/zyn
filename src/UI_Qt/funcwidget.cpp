#include "funcwidget.h"
#include "ui_funcwidget.h"

FuncWidget::FuncWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FuncWidget)
{
	ui->setupUi(this);
}

FuncWidget::~FuncWidget()
{
	delete ui;
}
