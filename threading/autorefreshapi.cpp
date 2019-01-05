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
	connect(api, SIGNAL(spotifyPlayingTrackFetched(Track)), this, SIGNAL(spotifyPlayingTrackFetched(Track)));
	
	connect(api, SIGNAL(geniusLinkedChanged(bool)), this, SIGNAL(geniusLinkedChanged(bool)));
	connect(api, SIGNAL(geniusLinkingSucceeded()), this, SIGNAL(geniusLinkingSucceeded()));
	connect(api, SIGNAL(geniusLinkingFailed()), this, SIGNAL(geniusLinkingFailed()));
	connect(api, SIGNAL(geniusOpenBrowser(QUrl)), this, SIGNAL(geniusOpenBrowser(QUrl)));
	connect(api, SIGNAL(geniusCloseBrowser()), this, SIGNAL(geniusCloseBrowser()));
	connect(api, SIGNAL(geniusLyricsFetched(Lyrics)), this, SIGNAL(geniusLyricsFetched(Lyrics)));
}

void AutoRefreshAPI::run() {
	if (t_refresh == nullptr) {
#ifdef QT_DEBUG
		cout << "AutoRefreshAPI> Connecting to APIs..." << endl;
#endif
		
		api->connectToSpotify();
		api->connectToGenius();
		
		t_refresh = new QTimer(this);
		t_refresh->setSingleShot(true);
		connect(t_refresh, SIGNAL(timeout()), this, SLOT(run()));
		t_refresh->start(pref->getRefreshTimeout());
		
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
		api->getLyrics();
		t_refresh->start(pref->getRefreshTimeout());
		emit refreshFinished();
	}
}

void AutoRefreshAPI::pause() {
	paused = true;
}

void AutoRefreshAPI::resume() {
	paused = false;
}

void AutoRefreshAPI::stop() {
	continueThread = false;
	t_refresh->stop();
}
