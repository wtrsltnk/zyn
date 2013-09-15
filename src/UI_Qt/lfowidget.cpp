#include "lfowidget.h"
#include "ui_lfowidget.h"

LfoWidget::LfoWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LfoWidget)
{
	ui->setupUi(this);
}

LfoWidget::~LfoWidget()
{
	delete ui;
}
