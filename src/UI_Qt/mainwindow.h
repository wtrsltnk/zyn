#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//class AddSynthWidget;
class QMdiSubWindow;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	QMdiSubWindow* addSynthWindow;

public slots:
	void toggleAddSynth(bool visible);
	void togglePiano(bool visible);

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
