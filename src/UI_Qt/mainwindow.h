#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include "qtoscnode.h"
#include "qtoscpane.h"

class QOscMdiSubWindow;
class AddSynthWidget;
class QMdiSubWindow;
class TreeWidget;
class VkWidget;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow, QtOscNode
{
	Q_OBJECT

	QOscDefaultPane<QTabWidget> *addSynthWindow = nullptr,
		*treeDlg = nullptr,
		*vkWindow = nullptr;
	AddSynthWidget* addSynth = nullptr;
	TreeWidget* treeWidget = nullptr;
	VkWidget* vkWidget = nullptr;

	void toggleWin(QWidget* content, bool visible);
private slots:
	inline void exit() { close(); }
	inline void toggleFullScreen( bool full ) {
		if(full) showFullScreen(); else showNormal();
	}
	void toggleParameterTree(bool visible);
	void helpAbout();
	void helpAboutQt();
	void toggleAddSynth(bool visible);
	void togglePiano(bool visible);
	void makeAllChildren();

public:
	void close();
	explicit MainWindow(Fl_Osc_Interface *osc, QWidget *parent = 0);
	~MainWindow();
	
private:
	Ui::MainWindow *ui;
	void OSC_raw(const char *msg);
};

#endif // MAINWINDOW_H
