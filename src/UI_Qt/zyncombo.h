#ifndef ZYNCOMBO_H
#define ZYNCOMBO_H

#include <QWidget>

namespace Ui {
class ZynCombo;
}

class ZynCombo : public QWidget
{
	Q_OBJECT
	
public:
	explicit ZynCombo(QWidget *parent = 0);
	~ZynCombo();
	
private:
	Ui::ZynCombo *ui;
};

#endif // ZYNCOMBO_H
