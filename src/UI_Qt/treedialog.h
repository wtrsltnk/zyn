#ifndef TREEDIALOG_H
#define TREEDIALOG_H

#include <QDialog>

namespace Ui {
class TreeDialog;
}
namespace rtosc {
	class Ports;
}
class QTreeWidgetItem;

class TreeDialog : public QDialog
{
	Q_OBJECT
	
	void recursiveAdd(const rtosc::Ports* ports, QTreeWidgetItem* parentItem);
public:
	explicit TreeDialog(QWidget *parent = 0);
	~TreeDialog();
	
private:
	Ui::TreeDialog *ui;
};

#endif // TREEDIALOG_H
