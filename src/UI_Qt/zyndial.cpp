#include <QDebug>

#include "../Misc/Master.h"

#include "zyndial.h"
#include "ui_zyndial.h"

ZynDial::ZynDial(QWidget *parent) :
	QWidget(parent),
	meta("amp_env/av::f"),
	ui(new Ui::ZynDial)
{
	ui->setupUi(this);


	rtosc::Ports& p = Master::ports;
	//qDebug() << p.apropos("part0/kit0/adpars/");

	//if(!meta["min"])
	// qDebug() << "scale not found!";

	ui->label->setText(p.apropos("part0/kit0/adpars/")->metadata);
}

ZynDial::~ZynDial()
{
	delete ui;
}
