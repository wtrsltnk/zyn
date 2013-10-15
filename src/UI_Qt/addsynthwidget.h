#ifndef ADDSYNTHWIDGET_H
#define ADDSYNTHWIDGET_H

#include "qtoscwidget.h"

namespace Ui {
	class AddSynthWidget;
}

class AddSynthWidget : public QtOscWidget
{
	Q_OBJECT
	
public:
	explicit AddSynthWidget(QtOscObject& parent);
	~AddSynthWidget();
	
private:
	Ui::AddSynthWidget *ui;
};

#endif // ADDSYNTHWIDGET_H
