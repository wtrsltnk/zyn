#ifndef ADDSYNTHWIDGET_H
#define ADDSYNTHWIDGET_H

#include "qtoscnode.h"

namespace Ui {
	class AddSynthWidget;
}

class AddSynthWidget : public QtOscWidget
{
	Q_OBJECT
	
public:
	explicit AddSynthWidget(QtOscWidget *parent = 0);
	~AddSynthWidget();
	void makeAllChildren();
	
private:
	Ui::AddSynthWidget *ui;
};

#endif // ADDSYNTHWIDGET_H
