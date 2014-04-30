#include "lfoenvwidget.h"
#include "ui_lfoenvwidget.h"

//#include <QDial>
//#include <QLabel>

// rParamF(foobar, rMap(min, -1.0), rMap(max, 20.0), "some port")

/*void LfoEnvWidget::init(QDial* dial, QLabel* label, const char* _loc)
{
	rtosc::Ports& p = Master::ports;
	QString loc = path + _loc;
	qDebug()<< loc;
	qDebug() << p.apropos(path.toAscii().data()	)->name;
	qDebug() << p.apropos(loc.toAscii().data())->name;
	qDebug() << p.apropos(loc.toAscii().data())->metadata;


	rtosc::Port::MetaContainer meta(p.apropos(loc.toAscii().data())->metadata);
	for(const auto x : meta)
	 qDebug() << " ... contains property " << x.title;

	//dial->setMinimum(atof(meta["min"]));
	//dial->setMaximum(atof(meta["max"]));
	assert(meta["min"] != NULL);
	qDebug() << "min" << meta["min"];
	qDebug() << "max" << meta["max"];

	label->setText(meta["documentation"]);

	connect(dial, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
	signalMapper->setMapping(dial, loc); // TODO: loc is local, a problem?
}*/

#include "QDebug" // TODO

LfoEnvWidget::LfoEnvWidget(QWidget *parent) :
	QtOscWidget(parent),
	ui(new Ui::LfoEnvWidget)
{
	ui->setupUi(this);
//	connect(signalMapper, SIGNAL(mapped(QString)), this, SIGNAL(clicked(QString)));

}

LfoEnvWidget::~LfoEnvWidget()
{
	delete ui;
}

void LfoEnvWidget::makeAllChildren(QtOscNode *dest, const char *_loc)
{
	//qDebug() << "this now: " << this;
	initDial(ui->dial, ui->label, "Pintensity"); // TODO: is this correct here? or makeChild()?
/*	ui->label->setText("??");
	qDebug() << ui->label->text();
	setStyleSheet("background-color:green;");
	show();
	update();
	ui->label->update();
	ui->label->show();*/

}
