#ifndef WEBAPI_H
#define WEBAPI_H

#include <iostream>
#include <QObject>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QRegularExpression>
#include <QTextDocument>

#include "lib/O2/o2.h"
#include "lib/O2/o2spotify.h"
#include "lib/O2/o2requestor.h"

#include "models/spotifytrack.h"
#include "models/geniustrack.h"
#include "threading/barrier.h"
#include "oauth/oauthdialog.h"
#include "r.h"

using namespace std;

// TODO: Use O2Spotify https://github.com/pipacs/o2/blob/master/src/o2spotify.h

class WebAPI : public QObject
{
	Q_OBJECT
public:
	explicit WebAPI(QObject *parent = nullptr);
	~WebAPI();
	
	void connectToSpotify();
	SpotifyTrack getPlayingTrack();
	
	void connectToGenius();
	QList<GeniusTrack> getLyricsList(const SpotifyTrack& track);
	QList<GeniusTrack> getLyricsList();
	
	QJsonArray getSearchList(const SpotifyTrack& track);
	
	QString getHtmlLyrics(QString lyricsPath);
	
	QString parseLyrics(QString html);
	
signals:
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
	void geniusLyricsListFetched(QList<GeniusTrack>);
	
private slots:
	void onSpotifyLinkedChanged();
	void onSpotifyLinkingSucceeded();
	void onSpotifyLinkingFailed();
	void onSpotifyOpenBrowser(const QUrl& url);
	void onSpotifyCloseBrowser();
	
	void onGeniusLinkedChanged();
	void onGeniusLinkingSucceeded();
	void onGeniusLinkingFailed();
	void onGeniusOpenBrowser(const QUrl& url);
	void onGeniusCloseBrowser();
	
	void onRequestFinished(int code, QNetworkReply::NetworkError error, QByteArray data);
	
private:
	O2Spotify* o2_spotify;
	O2* o2_genius;
	
	QEventLoop loopSpotify;
	QEventLoop loopGenius;
	QString bufferSpotifyPlayingTrackData;
	QString bufferGeniusSongInfo;
	
	OAuthDialog* webdialog = nullptr;
	
	int requestSpotifyPlayingTrackId;
	int requestGeniusSongInfoId;
};

#endif // WEBAPI_H
