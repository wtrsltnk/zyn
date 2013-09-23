#ifndef ENVWIDGET_H
#define ENVWIDGET_H

#include <QWidget>

namespace Ui {
class EnvWidget;
}

class EnvWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit EnvWidget(QWidget *parent = 0);
	~EnvWidget();
	
private:
	Ui::EnvWidget *ui;
};

#endif // ENVWIDGET_H
