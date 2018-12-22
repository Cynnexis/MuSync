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

#include "lib/O2/o2.h"
#include "lib/O2/o2requestor.h"

#include "models/track.h"

using namespace std;

class WebAPI : public QObject
{
	Q_OBJECT
public:
	explicit WebAPI(QObject *parent = nullptr);
	
	void connectToSpotify();
	
	void getPlayingTrack();
	
signals:
	void spotifyLinkedChanged(bool);
	void spotifyLinkingSucceeded();
	void spotifyLinkingFailed();
	void spotifyOpenBrowser(QUrl);
	void spotifyCloseBrowser();
	void spotifyPlayingTrackFetched(Track);
	
private slots:
	void onSpotifyLinkedChanged();
	void onSpotifyLinkingSucceeded();
	void onSpotifyLinkingFailed();
	void onSpotifyOpenBrowser(const QUrl& url);
	void onSpotifyCloseBrowser();
	void onSpotifyFinished(int code, QNetworkReply::NetworkError error, QByteArray data);
	
private:
	O2* o2_spotify;
	
	QString getCode(const QString& filename);
	
	int requestSpotifyPlayingTrackId;
};

#endif // WEBAPI_H
