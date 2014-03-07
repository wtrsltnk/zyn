#ifndef ZYNSPINBOX_H
#define ZYNSPINBOX_H

#include <QWidget>

namespace Ui {
	class ZynSpinBox;
}

class ZynSpinBox : public QWidget
{
	Q_OBJECT
	
public:
	explicit ZynSpinBox(QWidget *parent = 0);
	~ZynSpinBox();
	
private:
	Ui::ZynSpinBox *ui;
};

#endif // ZYNSPINBOX_H
