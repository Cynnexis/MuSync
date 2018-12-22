#ifndef WEBAPI_H
#define WEBAPI_H

#include <iostream>
#include <QObject>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>

#include "lib/O2/o2.h"
#include "lib/O2/o2requestor.h"

using namespace std;

class WebAPI : public QObject
{
	Q_OBJECT
public:
	explicit WebAPI(QObject *parent = nullptr);
	
	void connectToSpotify();
	
	void getPlayingTrack();
	
signals:
	void linkedChanged(bool);
	void linkingSucceeded();
	void linkingFailed();
	void openBrowser(QUrl);
	void closeBrowser();
	
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
