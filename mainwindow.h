#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QObject>
#include <QMainWindow>

#include "oauth/webapi.h"

using namespace std;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	
private slots:
	void on_pb_playingTrack_clicked();
	
private:
	Ui::MainWindow *ui;
	
	WebAPI* api;
};

#endif // MAINWINDOW_H
