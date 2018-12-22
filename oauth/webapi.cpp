#include "webapi.h"

WebAPI::WebAPI(QObject *parent) : QObject(parent) {
	// Get the spotify
	o2_spotify = new O2(this);
	connect(o2_spotify, SIGNAL(linkedChanged()), this, SLOT(onSpotifyLinkedChanged()));
	connect(o2_spotify, SIGNAL(linkingSucceeded()), this, SLOT(onSpotifyLinkingSucceeded()));
	connect(o2_spotify, SIGNAL(linkingFailed()), this, SLOT(onSpotifyLinkingFailed()));
	connect(o2_spotify, SIGNAL(openBrowser(QUrl)), this, SLOT(onSpotifyOpenBrowser(QUrl)));
	connect(o2_spotify, SIGNAL(closeBrowser()), this, SLOT(onSpotifyCloseBrowser()));
	
	connectToSpotify();
}

void WebAPI::connectToSpotify() {
	// Get the client id and secret from file
	QString spotifyClientId = this->getCode(":/text/spotify-client-id");
	QString spotifyClientSecret = this->getCode(":/text/spotify-client-secret");
	
	o2_spotify->setClientId(spotifyClientId);
	o2_spotify->setClientSecret(spotifyClientSecret);
	o2_spotify->setScope("user-read-currently-playing");
	o2_spotify->setRequestUrl("https://accounts.spotify.com/authorize");
	o2_spotify->setRefreshTokenUrl("https://accounts.spotify.com/api/token");
	o2_spotify->setTokenUrl("https://accounts.spotify.com/api/token");
	o2_spotify->setLocalPort(6814);
	
	o2_spotify->link();
}

void WebAPI::getPlayingTrack() {
	cout << "Spotify> Getting playing back..." << endl;
	// If not connected, force to connect
	if (!o2_spotify->linked()) {
		cout << "Spotify> Oups! Not connected yet..." << endl;
		disconnect(o2_spotify, SIGNAL(linkedChanged()), this, SLOT(onSpotifyLinkedChanged()));
		
		QTimer timer;
		timer.setSingleShot(true);
		QEventLoop loop;
		connect(o2_spotify, SIGNAL(linkedChanged()), &loop, SLOT(quit()));
		connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
		
		cout << "Spotify> Oups! Linking in progress..." << endl;
		o2_spotify->link();
		timer.start(120000); // 2m of timeout to connect
		loop.exec();
		cout << "Spotify> Oups! Linking done." << endl;
		
		// Reconnect the slot
		connect(o2_spotify, SIGNAL(linkedChanged()), this, SLOT(onSpotifyLinkedChanged()));
		
		// If timeout, throw an exception
		if (!timer.isActive())
			throw "Not connected to the Spotify API.";
	}
	
	cout << "Spotify> Sending request..." << endl;
	QNetworkRequest request = QNetworkRequest(QUrl("https://api.spotify.com/v1/me/player/currently-playing"));
	QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
	O2Requestor *requestor = new O2Requestor(mgr, o2_spotify, this);
	connect(requestor, SIGNAL(finished(int, QNetworkReply::NetworkError, QByteArray)), this, SLOT(onSpotifyFinished(int, QNetworkReply::NetworkError, QByteArray)));	
	requestSpotifyPlayingTrackId = requestor->get(request);
	cout << "Spotify> Getting user channel info... Please wait." << endl;
}

/* PRIVATE SLOTS */

void WebAPI::onSpotifyLinkedChanged() {
	cout << "Spotify> Linked changed: " << (o2_spotify->linked() ? "linked" : "unlinked") << endl;
}

void WebAPI::onSpotifyLinkingSucceeded() {
	cout << "Spotify> Login has succeded" << endl;
	getPlayingTrack();
}

void WebAPI::onSpotifyLinkingFailed() {
	cout << "Spotify> Login has failed" << endl;
}

void WebAPI::onSpotifyOpenBrowser(const QUrl& url) {
	cout << "Spotify> You need to connect to " << url.toString().toStdString() << endl;
	QDesktopServices::openUrl(url);
}

void WebAPI::onSpotifyCloseBrowser() {
	cout << "Spotify> You may close the browser" << endl;
}

void WebAPI::onSpotifyFinished(int code, QNetworkReply::NetworkError error, QByteArray data) {
	cout << "Spotify> Finished!" << endl;
	
	if (error != QNetworkReply::NoError)
		throw "Spotify> Error while fetching data from API.";
	
	if (code == requestSpotifyPlayingTrackId) {
		cout << "Spotify> Got Spotify track!" << endl;
		cout << QString(data).toStdString() << endl;
	}
}

/* PRIVATE FUNCTION */

QString WebAPI::getCode(const QString& filename) {
	QFile f(filename);
	
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		fprintf(stderr, "Cannot open the credential files.\n");
		qApp->exit(-1);
	}
	
	QString content = f.readAll().replace("\n", "");
	
	f.close();
	
	return content;
}
