#include "../Misc/Master.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "treedialog.h"
#include "ui_treedialog.h"

TreeDialog::TreeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TreeDialog)
{
	ui->setupUi(this);

	const rtosc::Ports* p = &Master::ports;

	new QTreeWidgetItem( ui->treeWidget, QStringList() << "(root)" );
	recursiveAdd(p, ui->treeWidget->topLevelItem(0));
}

TreeDialog::~TreeDialog()
{
	delete ui;
}


void TreeDialog::recursiveAdd(const rtosc::Ports* ports, QTreeWidgetItem* parentItem)
{
	if(ports)
	for(const rtosc::Port& port : *ports)
	{
		QTreeWidgetItem* child = new QTreeWidgetItem(QStringList() << port.name);
		parentItem->addChild(child);
		recursiveAdd(port.ports, child);
	}
}
