#ifndef QTOSCPANE_H
#define QTOSCPANE_H

#include <QTabWidget>
#include <QMdiSubWindow>
#include "../UI/Fl_Osc_Pane.H"

class QWidget;

//! @file Contains classes the inherit from Fl_Osc_Pane

template<class QtClass>
class QOscDefaultPane : public Fl_Osc_Pane, public QtClass
{
    public:
	QOscDefaultPane() {}
	void init(Fl_Osc_Interface *osc_, std::string loc_) {
		osc   = osc_;
		base = loc_;
	}

	virtual std::string loc(void) const { return base; }
	virtual void rebase(std::string new_base) {
		nested_rebase(
		static_cast<QtClass*>(this), new_base);
	}

};

using QOscTab = QOscDefaultPane<QTabWidget>;

#if 0
class QOscMdiSubWindow : public Fl_Osc_Pane, public QWidget
{
    public:
	QOscMdiSubWindow(QWidget *parent = nullptr);
	void init(Fl_Osc_Interface *osc_, std::string loc_);
	virtual std::string loc(void) const { return base; }
	virtual void rebase(std::string new_base);

};
#endif

#endif // QTOSCPANE_H
