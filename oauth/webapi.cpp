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
	o2_spotify->setTokenUrl("https://accounts.spotify.com/api/token");
	o2_spotify->setRefreshTokenUrl("https://accounts.spotify.com/api/token");
	o2_spotify->setLocalPort(6814);
	
	o2_spotify->link();
}

void WebAPI::getPlayingTrack() {
	// If not connected, force to connect
	if (!o2_spotify->linked()) {
		disconnect(o2_spotify, SIGNAL(linkedChanged()), this, SLOT(onSpotifyLinkedChanged()));
		
		QTimer timer;
		timer.setSingleShot(true);
		QEventLoop loop;
		connect(o2_spotify, SIGNAL(linkedChanged()), &loop, SLOT(quit()));
		connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
		
		o2_spotify->link();
		timer.start(120000); // 2m of timeout to connect
		loop.exec();
		
		// Reconnect the slot
		connect(o2_spotify, SIGNAL(linkedChanged()), this, SLOT(onSpotifyLinkedChanged()));
		
		// If timeout, throw an exception
		if (!timer.isActive())
			throw "Not connected to the Spotify API.";
	}
	
	QNetworkRequest request = QNetworkRequest(QUrl("https://api.spotify.com/v1/me/player/currently-playing"));
	QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
	O2Requestor *requestor = new O2Requestor(mgr, o2_spotify, this);
	connect(requestor, SIGNAL(finished(int, QNetworkReply::NetworkError, QByteArray)), this, SLOT(onSpotifyFinished(int, QNetworkReply::NetworkError, QByteArray)));	
	requestSpotifyPlayingTrackId = requestor->get(request);
}

/* PRIVATE SLOTS */

void WebAPI::onSpotifyLinkedChanged() {
	emit spotifyLinkedChanged(o2_spotify->linked());
}

void WebAPI::onSpotifyLinkingSucceeded() {
	emit spotifyLinkingSucceeded();
	getPlayingTrack();
}

void WebAPI::onSpotifyLinkingFailed() {
	cout << "Spotify> Login has failed" << endl;
	emit spotifyLinkingFailed();
}

void WebAPI::onSpotifyOpenBrowser(const QUrl& url) {
	emit spotifyOpenBrowser(url);
	QDesktopServices::openUrl(url);
}

void WebAPI::onSpotifyCloseBrowser() {
	emit spotifyCloseBrowser();
}

void WebAPI::onSpotifyFinished(int code, QNetworkReply::NetworkError error, QByteArray data) {
	if (error != QNetworkReply::NoError)
		throw "Spotify> Error while fetching data from API.";
	
	if (code == requestSpotifyPlayingTrackId) {
		cout << QString(data).toStdString() << endl;
		
		if (QString(data) == "") {
			emit spotifyPlayingTrackFetched(Track());
			return;
		}
		
		QJsonDocument doc = QJsonDocument::fromJson(data);
		QJsonObject json = doc.object();
		
		if (json.contains("item"))
			json = json["item"].toObject();
		
		QString name = json["name"].toString("");
		QJsonArray jsonArtists = json["artists"].toArray();
		QStringList artists;
		for (QJsonValue jsonArtist : jsonArtists)
			artists << jsonArtist.toObject()["name"].toString("");
		
		artists.removeAll("");
		
		QString albumName = json["album"].toObject()["name"].toString("");
		
		Track track = Track(name, artists, albumName, this);
		
		cout << "Spotify> " << track.toString().toStdString() << endl;
		
		emit spotifyPlayingTrackFetched(track);
	}
}

/* PRIVATE FUNCTION */

QString WebAPI::getCode(const QString& filename) {
	QFile f(filename);
	
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		throw "Cannot open the credential files.\n";
	
	QString content = f.readAll().replace("\r\n", "").replace("\r", "").replace("\n", "");
	
	f.close();
	
	return content;
}
