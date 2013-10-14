#ifndef LFOENVWIDGET_H
#define LFOENVWIDGET_H

#include <QWidget>
#include "qtoscinterface.h"
#include "qtoscnode.h"

class QDial;
class QLabel;
class QSignalMapper;

namespace Ui {
	class LfoEnvWidget;
}

class LfoEnvWidget : public QWidget, public QtOscNode
{
	Q_OBJECT
public:
	explicit LfoEnvWidget(QWidget *parent = 0);
	~LfoEnvWidget();
	
	//void init(QDial *dial, QLabel *label, const char *loc);
private:
	Ui::LfoEnvWidget *ui;
};

#endif // LFOENVWIDGET_H
