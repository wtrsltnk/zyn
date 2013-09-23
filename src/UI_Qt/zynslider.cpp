#include "zynslider.h"
#include "ui_zynslider.h"

ZynSlider::ZynSlider(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ZynSlider)
{
	ui->setupUi(this);
	ui->horizontalSlider
}

ZynSlider::~ZynSlider()
{
	delete ui;
}
