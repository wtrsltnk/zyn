#include "zynspinbox.h"
#include "ui_zynspinbox.h"

ZynSpinBox::ZynSpinBox(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ZynSpinBox)
{
	ui->setupUi(this);
}

ZynSpinBox::~ZynSpinBox()
{
	delete ui;
}
