#ifndef ADDSYNTHWIDGET_H
#define ADDSYNTHWIDGET_H

#include <QWidget>

namespace Ui {
	class AddSynthWidget;
}

class AddSynthWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit AddSynthWidget(QWidget *parent = 0);
	~AddSynthWidget();
	
private:
	Ui::AddSynthWidget *ui;
};

#endif // ADDSYNTHWIDGET_H
