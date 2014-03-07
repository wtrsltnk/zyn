#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "../Misc/Master.h"
#include "treewidget.h"
#include "ui_treewidget.h"

TreeWidget::TreeWidget(QtOscWidget *parent) :
	QtOscWidget(parent),
	ui(new Ui::TreeWidget)
{
	ui->setupUi(this);

	const rtosc::Ports* p = &Master::ports;

	new QTreeWidgetItem( ui->treeWidget, QStringList() << "(root)" );
	recursiveAdd(p, ui->treeWidget->topLevelItem(0));
}

TreeWidget::~TreeWidget()
{
	delete ui;
}


void TreeWidget::recursiveAdd(const rtosc::Ports *ports, QTreeWidgetItem *parentItem)
{
	if(ports)
	for(const rtosc::Port& port : *ports)
	{
		QTreeWidgetItem* child = new QTreeWidgetItem(QStringList() << port.name);
		parentItem->addChild(child);
		recursiveAdd(port.ports, child);
	}
}
