#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Forward declarations.
class Ui_mainWindow;
class Project;

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private:
	Ui_mainWindow* ui;
	Project* project;
};

#endif // MAINWINDOW_H
