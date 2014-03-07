#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include "qtoscnode.h"

namespace Ui {
	class TreeWidget;
}
namespace rtosc {
	class Ports;
}
class QTreeWidgetItem;
class QtOscWidget;

class TreeWidget : public QtOscWidget
{
	Q_OBJECT
	
	void recursiveAdd(const rtosc::Ports* ports, QTreeWidgetItem* parentItem);
public:
	explicit TreeWidget(QtOscWidget* parent = 0);
	~TreeWidget();
	
private:
	Ui::TreeWidget *ui;
};

#endif // TREEWIDGET_H
