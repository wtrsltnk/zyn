#include "zyncheckbox.h"
#include "ui_zyncheckbox.h"

ZynCheckBox::ZynCheckBox(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ZynCheckBox)
{
	ui->setupUi(this);
}

ZynCheckBox::~ZynCheckBox()
{
	delete ui;
}
