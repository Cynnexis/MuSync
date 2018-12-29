#include "webapi.h"

WebAPI::WebAPI(QObject *parent) : QObject(parent) {
	// Get the spotify API
	o2_spotify = new O2(this);
#ifndef QT_DEBUG
	o2_spotify->setIgnoreSslErrors(true);
#endif
	connect(o2_spotify, SIGNAL(linkedChanged()), this, SLOT(onSpotifyLinkedChanged()));
	connect(o2_spotify, SIGNAL(linkingSucceeded()), this, SLOT(onSpotifyLinkingSucceeded()));
	connect(o2_spotify, SIGNAL(linkingFailed()), this, SLOT(onSpotifyLinkingFailed()));
	connect(o2_spotify, SIGNAL(openBrowser(QUrl)), this, SLOT(onSpotifyOpenBrowser(QUrl)));
	connect(o2_spotify, SIGNAL(closeBrowser()), this, SLOT(onSpotifyCloseBrowser()));
	
	// Get the genius API
	o2_genius = new O2(this);
#ifndef QT_DEBUG
	o2_genius->setIgnoreSslErrors(true);
#endif
	connect(o2_genius, SIGNAL(linkedChanged()), this, SLOT(onGeniusLinkedChanged()));
	connect(o2_genius, SIGNAL(linkingSucceeded()), this, SLOT(onGeniusLinkingSucceeded()));
	connect(o2_genius, SIGNAL(linkingFailed()), this, SLOT(onGeniusLinkingFailed()));
	connect(o2_genius, SIGNAL(openBrowser(QUrl)), this, SLOT(onGeniusOpenBrowser(QUrl)));
	connect(o2_genius, SIGNAL(closeBrowser()), this, SLOT(onGeniusCloseBrowser()));
	
	connectToSpotify();
	connectToGenius();
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
	if (!o2_spotify->linked())
		throw "Not connected to the Spotify API.";
	
	QNetworkRequest request = QNetworkRequest(QUrl("https://api.spotify.com/v1/me/player/currently-playing"));
	QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
	O2Requestor *requestor = new O2Requestor(mgr, o2_spotify, this);
	connect(requestor, SIGNAL(finished(int, QNetworkReply::NetworkError, QByteArray)), this, SLOT(onRequestFinished(int, QNetworkReply::NetworkError, QByteArray)));	
	requestSpotifyPlayingTrackId = requestor->get(request);
}

void WebAPI::connectToGenius() {
	QString geniusClientId = this->getCode(":/text/genius-client-id");
	QString geniusClientSecret = this->getCode(":/text/genius-client-secret");
	
	o2_genius->setClientId(geniusClientId);
	o2_genius->setClientSecret(geniusClientSecret);
	o2_genius->setRequestUrl("https://api.genius.com/oauth/authorize");
	o2_genius->setTokenUrl("https://api.genius.com/oauth/token");
	o2_genius->setRefreshTokenUrl("https://api.genius.com/oauth/token");
	o2_genius->setLocalPort(6815);
	
	o2_genius->link();
}

void WebAPI::getLyrics(const Track& track) {
	if (track.getName() == "" || track.getArtists().isEmpty())
		return;
	
#ifdef QT_DEBUG
	cout << "Genius> Fetching lyrics for " << track.getName().toStdString() << endl;
#endif
	
	if (!o2_genius->linked())
		throw "Not connected to the Genius API.";
	
	QNetworkRequest request = QNetworkRequest(QUrl("https://api.genius.com/search?q=" + QUrl::toPercentEncoding(track.getName() + " " + track.getArtists().join(","))));
	QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
	O2Requestor *requestor = new O2Requestor(mgr, o2_genius, this);
	connect(requestor, SIGNAL(finished(int, QNetworkReply::NetworkError, QByteArray)), this, SLOT(onRequestFinished(int, QNetworkReply::NetworkError, QByteArray)));	
	requestGeniusSongInfoId = requestor->get(request);
}

void WebAPI::getLyrics() {
	getPlayingTrack();
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

void WebAPI::onGeniusLinkedChanged() {
	emit geniusLinkedChanged(o2_spotify->linked());
}

void WebAPI::onGeniusLinkingSucceeded() {
	emit geniusLinkingSucceeded();
	//geniusBarrier->wait();
}

void WebAPI::onGeniusLinkingFailed() {
	cout << "Genius> Login has failed" << endl;
	emit geniusLinkingFailed();
}

void WebAPI::onGeniusOpenBrowser(const QUrl& url) {
	emit geniusOpenBrowser(url);
	QDesktopServices::openUrl(url);
}

void WebAPI::onGeniusCloseBrowser() {
	emit geniusCloseBrowser();
}

void WebAPI::onRequestFinished(int code, QNetworkReply::NetworkError error, QByteArray data) {
	if (error != QNetworkReply::NoError)
		throw "Error while fetching data from API.";
	
	if (code == requestSpotifyPlayingTrackId) {
#ifdef QT_DEBUG
		cout << "Spotify> " << QString(data).toStdString() << endl;
#endif
		
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
		
		QJsonArray thumbnails = json["album"].toObject()["images"].toArray();
		QString thumbnailUrl = "";
		
		if (thumbnails.size() > 0)
			thumbnailUrl = thumbnails[0].toObject()["url"].toString(thumbnailUrl);
		
		Track track = Track(name, artists, albumName, thumbnailUrl, false, this);
		
#ifdef QT_DEBUG
		cout << "Spotify> " << track.toString().toStdString() << endl;
#endif
		
		// Emit the track
		emit spotifyPlayingTrackFetched(track);
		
		// Fetch the lyrics with Genius API
		getLyrics(track);
	}
	else if (code == requestGeniusSongInfoId) {
		QString lyrics = "No lyrics found";
		if (QString(data) == "") {
			emit geniusLyricsFetched(lyrics);
			return;
		}
		
		QJsonDocument doc = QJsonDocument::fromJson(data);
		QJsonObject json = doc.object();
		
		QJsonArray hits = json["response"].toObject()["hits"].toArray();
		
		bool found = false;
		QJsonObject firstResult;
		
		for (QJsonValue h : hits) {
			if (h.toObject()["type"].toString("") == "song") {
				found = true;
				firstResult = h.toObject();
				break;
			}
		}
		
		// If a valid result have been found
		if (found) {
			// Get the URI to the lyrics
			QString firstResultPath = firstResult["result"].toObject()["path"].toString("");
			
			if (!firstResultPath.startsWith("/"))
				firstResultPath = "/" + firstResultPath;
			
			if (firstResultPath != "") {
				// Fetch the HTML page containing the lyrics
				QNetworkRequest request = QNetworkRequest(QUrl("https://genius.com" + firstResultPath));
				QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
				QNetworkReply* response = mgr->get(request);
				QEventLoop event;
				connect(response, SIGNAL(finished()), &event, SLOT(quit()));	
				event.exec();
				
				QString html = response->readAll();
				QStringList list = html.split("<div class=\"lyrics\">");
				
				// If no lyrics detected, return no lyrics
				if (list.length() == 0) {
					emit geniusLyricsFetched(lyrics);
					return;
				}
				
				html = list[1];
				list = html.split("<!--sse-->");
				
				// If no lyrics detected, return no lyrics
				if (list.length() == 0) {
					emit geniusLyricsFetched(lyrics);
					return;
				}
				
				html = list[1];
				list = html.split("<!--/sse-->");
				
				if (list.length() == 0) {
					emit geniusLyricsFetched(lyrics);
					return;
				}
				
				html = list[0];
				
				html = html.replace(QRegularExpression("<\\/?[^>]+>"), "");
				html = html.replace(QRegularExpression("(\\n|\\r\\n|\\n\\r|\\s)*\\0?$"), "");
				html = html.replace(QRegularExpression("^(\\n|\\r\\n|\\n\\r|\\s)*"), "");
				//QTextDocument text;
				//text.setHtml(html);
				//html = text.toPlainText();
				lyrics = html;
				
				//cout << "Lyrics> " << lyrics.toStdString() << endl;
				emit geniusLyricsFetched(lyrics);
			}
			// If no path found, return no lyrics
			else {
				emit geniusLyricsFetched(lyrics);
				return;
			}
		}
		// If no result found, return no lyrics
		else {
			emit geniusLyricsFetched(lyrics);
			return;
		}
	}
}

/* PRIVATE FUNCTION */

QString WebAPI::getCode(const QString& filename) const {
	QFile f(filename);
	
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		throw "Cannot open the credential files.\n";
	
	QString content = f.readAll().replace("\r\n", "").replace("\r", "").replace("\n", "");
	
	f.close();
	
	return content;
}
