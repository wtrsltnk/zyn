#include "zyncombo.h"
#include "ui_zyncombo.h"

ZynCombo::ZynCombo(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ZynCombo)
{
	ui->setupUi(this);
}

ZynCombo::~ZynCombo()
{
	delete ui;
}
