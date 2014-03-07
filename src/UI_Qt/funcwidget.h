#ifndef FUNCWIDGET_H
#define FUNCWIDGET_H

#include <QWidget>

namespace Ui {
	class FuncWidget;
}

class FuncWidget : public QWidget
{
	Q_OBJECT
	friend class AddSynthWidget;
public:
	explicit FuncWidget(QWidget *parent = 0);
	~FuncWidget();
	
private:
	Ui::FuncWidget *ui;
};

#endif // FUNCWIDGET_H
