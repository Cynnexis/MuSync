#include "autorefreshapi.h"

AutoRefreshAPI::AutoRefreshAPI(QObject* parent) : QObject(parent) {
	pref = Preferences::getInstance();
	
	api = new WebAPI();
	
	// Pass along all signals from WebAPI
	connect(api, SIGNAL(spotifyLinkedChanged(bool)), this, SIGNAL(spotifyLinkedChanged(bool)));
	connect(api, SIGNAL(spotifyLinkingSucceeded()), this, SIGNAL(spotifyLinkingSucceeded()));
	connect(api, SIGNAL(spotifyLinkingFailed()), this, SIGNAL(spotifyLinkingFailed()));
	connect(api, SIGNAL(spotifyOpenBrowser(QUrl)), this, SIGNAL(spotifyOpenBrowser(QUrl)));
	connect(api, SIGNAL(spotifyCloseBrowser()), this, SIGNAL(spotifyCloseBrowser()));
	connect(api, SIGNAL(spotifyPlayingTrackFetched(SpotifyTrack)), this, SIGNAL(spotifyPlayingTrackFetched(SpotifyTrack)));
	
	connect(api, SIGNAL(geniusLinkedChanged(bool)), this, SIGNAL(geniusLinkedChanged(bool)));
	connect(api, SIGNAL(geniusLinkingSucceeded()), this, SIGNAL(geniusLinkingSucceeded()));
	connect(api, SIGNAL(geniusLinkingFailed()), this, SIGNAL(geniusLinkingFailed()));
	connect(api, SIGNAL(geniusOpenBrowser(QUrl)), this, SIGNAL(geniusOpenBrowser(QUrl)));
	connect(api, SIGNAL(geniusCloseBrowser()), this, SIGNAL(geniusCloseBrowser()));
	connect(api, SIGNAL(geniusLyricsFetched(Lyrics)), this, SIGNAL(geniusLyricsFetched(Lyrics)));
	connect(api, SIGNAL(geniusLyricsListFetched(QList<Lyrics>)), this, SIGNAL(geniusLyricsListFetched(QList<Lyrics>)));
}

void AutoRefreshAPI::refresh() {
	if (t_refresh == nullptr) {
#ifdef QT_DEBUG
		cout << "AutoRefreshAPI> Connecting to APIs..." << endl;
#endif
		
		api->connectToSpotify();
		api->connectToGenius();
		
		t_refresh = new QTimer(this);
		t_refresh->setSingleShot(true);
		connect(t_refresh, SIGNAL(timeout()), this, SLOT(refresh()));
		t_refresh->start(100);
		
#ifdef QT_DEBUG
		cout << "AutoRefreshAPI> APIs connected." << endl;
#endif
		
		emit APIsConnected();
	}
	else {
#ifdef QT_DEBUG
		cout << "AutoRefreshAPI> Refreshing..." << endl;
#endif
		emit aboutToRefresh();
		t_refresh->stop();
		SpotifyTrack track = api->getPlayingTrack();
		if (track != lastTrackFetched) {
			lastTrackFetched = track;
			api->getLyricsList(track);
		}
		t_refresh->start(pref->getRefreshTimeout());
		emit refreshFinished();
	}
}

void AutoRefreshAPI::pause() {
	if (t_refresh != nullptr)
		t_refresh->stop();
}

void AutoRefreshAPI::resume() {
	if (t_refresh != nullptr)
		t_refresh->start(0);
}

void AutoRefreshAPI::stop() {
	if (t_refresh != nullptr)
		t_refresh->stop();
}

bool AutoRefreshAPI::isRunning()
{
	if (t_refresh != nullptr)
		return t_refresh->isActive();
	else
		return false;
}
