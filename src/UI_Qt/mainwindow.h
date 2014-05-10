#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
//#include "treedialog.h"
//#include "qtoscwidget.h" // TODO
#include "qtoscnode.h"

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

	QMdiSubWindow *addSynthWindow = nullptr,
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
	//inline void toggleAddSynth(bool v) { toggleWin(addSynthWindow, v); /*toggleWin(ui->mdiArea, addSynthWindow, v);*/ }
	void togglePiano(bool visible);
	void makeAllChildren(QtOscNode *dest, const char *_loc);

public:
	// template funcs are declared below
	//template<class T> void toggleWin(QMdiArea* mdiArea, T* mdiSubWindow, bool visible);
	void close();
	explicit MainWindow(Fl_Osc_Interface *osc, QWidget *parent = 0);
	~MainWindow();
	
private:
	Ui::MainWindow *ui;
	void OSC_raw(const char *msg);
};

/*template<class T> void MainWindow::toggleWin(QMdiArea* mdiArea, T* mdiSubWindow, bool visible)
{
	if(!mdiSubWindow){
		T* widget = new T();
		mdiSubWindow = mdiArea->addSubWindow(widget);
	}
	mdiSubWindow->setVisible(visible);
}*/


#endif // MAINWINDOW_H
