#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QObject>
#include <QMainWindow>
#include <QTimer>

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
	void getTrack(Track track);
	void getLyrics(QString lyrics);
	
	void onTrackNameChanged(QString name);
	void onTrackArtistsChanged(QStringList artists);
	void onTrackAlbumName(QString albumName);
	void onTrackThumbnailChanged(QPixmap thumbnail);
	
public slots:
	void refresh();
	
private slots:
	void on_actionRefresh_triggered();
	void on_actionExit_triggered();
	
private:
	Ui::MainWindow *ui;
	
	WebAPI* api;
	Track currentTrack;
	QTimer* timer;
	
	int refreshTimeout = 5000;
};

#endif // MAINWINDOW_H
