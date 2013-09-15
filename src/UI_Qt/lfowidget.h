#ifndef LFOWIDGET_H
#define LFOWIDGET_H

#include <QWidget>

namespace Ui {
	class LfoWidget;
}

class LfoWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit LfoWidget(QWidget *parent = 0);
	~LfoWidget();
	
private:
	Ui::LfoWidget *ui;
};

#endif // LFOWIDGET_H
