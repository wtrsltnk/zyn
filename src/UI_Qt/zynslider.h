#ifndef ZYNSLIDER_H
#define ZYNSLIDER_H

#include <QWidget>

namespace Ui {
class ZynSlider;
}

class ZynSlider : public QWidget
{
	Q_OBJECT
	
public:
	explicit ZynSlider(QWidget *parent = 0);
	~ZynSlider();
	
private:
	Ui::ZynSlider *ui;
};

#endif // ZYNSLIDER_H
