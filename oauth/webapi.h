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
#include "lib/O2/o2requestor.h"

#include "models/track.h"
#include "threading/barrier.h"

using namespace std;

class WebAPI : public QObject
{
	Q_OBJECT
public:
	explicit WebAPI(QObject *parent = nullptr);
	
	void connectToSpotify();
	void getPlayingTrack();
	
	void connectToGenius();
	void getLyrics(const Track& track);
	void getLyrics();
	
signals:
	void spotifyLinkedChanged(bool);
	void spotifyLinkingSucceeded();
	void spotifyLinkingFailed();
	void spotifyOpenBrowser(QUrl);
	void spotifyCloseBrowser();
	void spotifyPlayingTrackFetched(Track);
	
	void geniusLinkedChanged(bool);
	void geniusLinkingSucceeded();
	void geniusLinkingFailed();
	void geniusOpenBrowser(QUrl);
	void geniusCloseBrowser();
	void geniusLyricsFetched(QString);
	
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
	O2* o2_spotify;
	O2* o2_genius;
	
	QString getCode(const QString& filename) const;
	
	int requestSpotifyPlayingTrackId;
	int requestGeniusSongInfoId;
};

#endif // WEBAPI_H
