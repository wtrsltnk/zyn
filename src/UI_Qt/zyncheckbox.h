#ifndef ZYNCHECKBOX_H
#define ZYNCHECKBOX_H

#include <QWidget>

namespace Ui {
class ZynCheckBox;
}

class ZynCheckBox : public QWidget
{
	Q_OBJECT
	
public:
	explicit ZynCheckBox(QWidget *parent = 0);
	~ZynCheckBox();
	
private:
	Ui::ZynCheckBox *ui;
};

#endif // ZYNCHECKBOX_H
