#ifndef FILTERPARAMWIDGET_H
#define FILTERPARAMWIDGET_H

#include <QWidget>

namespace Ui {
class FilterParamWidget;
}

class FilterParamWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit FilterParamWidget(QWidget *parent = 0);
	~FilterParamWidget();
	
private:
	Ui::FilterParamWidget *ui;
};

#endif // FILTERPARAMWIDGET_H
