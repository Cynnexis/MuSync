#ifndef AUTOREFRESHAPI_H
#define AUTOREFRESHAPI_H

#include <iostream>
#include <QObject>
#include <QThread>
#include <QRunnable>

#include "oauth/webapi.h"
#include "preferences.h"
#include "models/spotifytrack.h"
#include "models/geniustrack.h"

using namespace std;

class AutoRefreshAPI : public QObject
{
	Q_OBJECT
	
public:
	AutoRefreshAPI(QObject* parent = nullptr);
	
public slots:
	void refresh();
	
	void pause();
	void resume();
	void stop();
	
public:
	bool isRunning();
	
signals:
	void APIsConnected();
	void aboutToRefresh();
	void refreshFinished();
	
	// Exact same signals from WebAPI
	void spotifyLinkedChanged(bool);
	void spotifyLinkingSucceeded();
	void spotifyLinkingFailed();
	void spotifyOpenBrowser(QUrl);
	void spotifyCloseBrowser();
	void spotifyPlayingTrackFetched(SpotifyTrack);
	
	void geniusLinkedChanged(bool);
	void geniusLinkingSucceeded();
	void geniusLinkingFailed();
	void geniusOpenBrowser(QUrl);
	void geniusCloseBrowser();
	//void geniusLyricsFetched(GeniusTrack);
	void geniusLyricsListFetched(QList<GeniusTrack>);
	
private:
	WebAPI* api = nullptr;
	QTimer* t_refresh = nullptr;
	
	Preferences* pref;
	
	SpotifyTrack lastTrackFetched;
};

#endif // AUTOREFRESHAPI_H
