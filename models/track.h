#ifndef TRACK_H
#define TRACK_H

#include <iostream>
#include <QObject>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include "artist.h"
#include "qartistlist.h"

using namespace std;

class Track : public QObject
{
	Q_OBJECT
public:
	Track(const QString& name = "",
		  const QStringList& artists = {},
		  const QString& albumName = "",
		  const QPixmap thumbnail = QPixmap(),
		  const QString& spotifyUri = "",
		  const QString& spotifyWebUrl = "",
		  QObject *parent = nullptr);
	Track(const QString& name,
		  const QStringList& artists,
		  const QString& albumName,
		  const QString thumbnailUrl,
		  const QString& spotifyUri,
		  const QString& spotifyWebUrl,
		  const bool& download = true,
		  QObject *parent = nullptr);
	Track(const Track& track);
	~Track();
	
private:
	void init(const QString& name,
			  const QStringList& artists,
			  const QString& albumName,
			  const QPixmap thumbnail,
			  const QString spotifyUri,
			  const QString spotifyWebUrl);
	
private slots:
	void thumbnailDownloadFinished(QNetworkReply* reply);
	
public:
	QString getName() const;
	void setName(const QString& value);
	
	QStringList getArtists() const;
	void setArtists(const QStringList& value);
	
	QString getAlbumName() const;
	void setAlbumName(const QString& value);
	
	QPixmap getThumbnail() const;
	void setThumbnail(const QPixmap& value);
	void setThumbnail(const QUrl& thumbnailUrl, const bool& download = true);
	void downloadThumbnail(const QUrl& thumbnailUrl);
	void downloadThumbnail();
	QUrl getThumbnailUrl() const;
	
	QString getSpotifyUri() const;
	void setSpotifyUri(const QString& value);
	
	QString getSpotifyWebUrl() const;
	void setSpotifyWebUrl(const QString& value);
	
	QString toString() const;
	
	bool operator==(const Track& that) const;
	bool operator!=(const Track& that) const;
	Track& operator=(Track that);
	
signals:
	void nameChanged(QString);
	void artistsChanged(QStringList);
	void albumNameChanged(QString);
	void thumbnailChanged(QPixmap);
	void spotifyUriChanged(QString);
	void spotifyWebUrlChanged(QString);
	
private:
	QString name;
	QStringList artists;
	QString albumName;
	QPixmap thumbnail;
	QString spotifyUri;
	QString spotifyWebUrl;
	
	QUrl thumbnailUrl;
};

#endif // TRACK_H
