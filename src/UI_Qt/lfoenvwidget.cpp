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


LfoEnvWidget::LfoEnvWidget(QWidget *parent) :
	QWidget(parent),
	QtOscNode(::osc)
	ui(new Ui::LfoEnvWidget)
{
	ui->setupUi(this);

	//init(ui->dial, ui->label, "Pfreq");

//	connect(signalMapper, SIGNAL(mapped(QString)), this, SIGNAL(clicked(QString)));

}

LfoEnvWidget::~LfoEnvWidget()
{
	delete ui;
}
