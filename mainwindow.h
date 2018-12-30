#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QtConcurrent>
#include <QFuture>

#include "oauth/webapi.h"
#include "oauth/oauthdialog.h"
#include "models/qartistlist.h"
#include "preferences.h"

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
	
private:
	void changeTitle(QString title = "");
	void changeTitle(Track track);
	
private slots:
	void getTrack(Track track);
	void getLyrics(QString lyrics);
	
	void onTrackNameChanged(QString name);
	void onTrackArtistsChanged(QArtistList artists);
	void onTrackAlbumName(QString albumName);
	void onTrackThumbnailChanged(QPixmap thumbnail);
	
	void connectAPIs();
	
public slots:
	void refresh();
	
	void requestOpenBrowser(const QUrl& url);
	void requestCloseBrowser();
	
private:
	void showEvent(QShowEvent* event);
	
private slots:
	void on_actionRefresh_triggered();
	void on_actionExit_triggered();
	
private:
	Ui::MainWindow *ui = nullptr;
	Preferences* pref = nullptr;
	OAuthDialog* webdialog = nullptr;
	
	WebAPI* api = nullptr;
	Track currentTrack;
	QThread* threadAPIs = nullptr;
	QTimer* timerRefresh = nullptr;
};

#endif // MAINWINDOW_H
