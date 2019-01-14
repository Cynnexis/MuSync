#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QAction>
#include <QSystemTrayIcon>

#include "oauth/oauthdialog.h"
#include "models/qartistlist.h"
#include "models/artist.h"
#include "preferences.h"
#include "threading/autorefreshapi.h"
#include "ui/dsettings.h"
#include "ui/loadingoverlay.h"
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
	void stopThreads();
	
protected:
	void closeEvent(QCloseEvent* event) override;
	
private slots:
	void getTrack(Track track);
	void getLyrics(Lyrics lyrics);
	
	void onTrackNameChanged(QString name);
	void onTrackArtistsChanged(QArtistList artists);
	void onTrackAlbumChanged(Album album);
	void onTrackThumbnailChanged(QPixmap thumbnail);
	
	void onLyricsLyricsChanged(QString lyrics);
	
public slots:
	void requestOpenBrowser(const QUrl& url);
	void requestCloseBrowser();
	
private:
	void showEvent(QShowEvent* event) override;
	
private slots:
	/* Tray slots*/
	void onTrayClicked(QSystemTrayIcon::ActivationReason reason);
	void onTrayMessageClicked();
	
	/* API slots */
	void onAPIsConnected();
	void onAboutToRefresh();
	
	/* Preferences slots */
	void onStartupBehaviourChanged(int startupBehaviour);
	void onStyleChanged(int style);
	
	/* Menu slots */	
	void on_actionRefresh_triggered();
	void on_actionResumePause_triggered();
	void on_actionSettings_triggered();
	void on_actionExit_triggered();
	
	void on_actionOpenTrackOnSpotifyApp_triggered();
	void on_actionOpenTrackOnSpotifyWeb_triggered();
	void on_actionOpenAlbumOnSpotifyApp_triggered();
	void on_actionOpenAlbumOnSpotifyWeb_triggered();
	void on_actionOpenLyricsOnGenius_triggered();
	
	void on_actionAboutMuSync_triggered();
	void on_actionAboutQt_triggered();
	
signals:
	void windowAboutToBeClosed();
	void pauseRequest();
	void resumeRequest();
	
private:
	static const int RESUMED = 0;
	static const int PAUSED = 1;
	
	Ui::MainWindow *ui = nullptr;
	Preferences* pref = nullptr;
	OAuthDialog* webdialog = nullptr;
	DSettings* dsettings = nullptr;
	QSystemTrayIcon* traySystem = nullptr;
	QMenu* trayMenu = nullptr;
	
	LoadingOverlay* loadingOverlay = nullptr;
	
	AutoRefreshAPI* refreshAPIs = nullptr;
	QThread* threadAPIs = nullptr;
	Track currentTrack;
	Lyrics currentLyrics;
	
	int refreshStatus = RESUMED;
};

#endif // MAINWINDOW_H
