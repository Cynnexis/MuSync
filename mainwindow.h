#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QThreadPool>
#include <QtConcurrent>
#include <QFuture>
#include <QMessageBox>
#include <QAction>

#include "oauth/oauthdialog.h"
#include "models/qartistlist.h"
#include "models/artist.h"
#include "preferences.h"
#include "threading/autorefreshapi.h"
#include "r.h"

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
	void getLyrics(Lyrics lyrics);
	
	void onTrackNameChanged(QString name);
	void onTrackArtistsChanged(QArtistList artists);
	void onTrackAlbumChanged(Album album);
	void onTrackThumbnailChanged(QPixmap thumbnail);
	
	void onLyricsLyricsChanged(QString lyrics);
	
	//void connectAPIs();
	
public slots:
	//void refresh();
	
	void requestOpenBrowser(const QUrl& url);
	void requestCloseBrowser();
	
private:
	void showEvent(QShowEvent* event);
	
private slots:
	void onAPIsConnected();
	void onAboutToRefresh();
	
	void on_actionRefresh_triggered();
	void on_actionExit_triggered();
	
	void on_actionOpenTrackOnSpotifyApp_triggered();
	void on_actionOpenTrackOnSpotifyWeb_triggered();
	void on_actionOpenAlbumOnSpotifyApp_triggered();
	void on_actionOpenAlbumOnSpotifyWeb_triggered();
	void on_actionOpenLyricsOnGenius_triggered();
	
	void on_actionAboutMuSync_triggered();
	void on_actionAboutQt_triggered();
	
private:
	Ui::MainWindow *ui = nullptr;
	Preferences* pref = nullptr;
	OAuthDialog* webdialog = nullptr;
	
	AutoRefreshAPI* refreshAPIs;
	QThread* threadAPIs;
	Track currentTrack;
	Lyrics currentLyrics;
};

#endif // MAINWINDOW_H
