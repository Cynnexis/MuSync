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
#include "album.h"

using namespace std;

class Track : public QObject
{
	Q_OBJECT
public:
	Track(const QString& name = "",
		  const QArtistList& artists = {},
		  const Album& album = Album(),
		  const QPixmap thumbnail = QPixmap(),
		  const int& trackNumber = 0,
		  const QString& spotifyUri = "",
		  const QString& spotifyWebUrl = "",
		  QObject *parent = nullptr);
	Track(const QString& name,
		  const QArtistList& artists,
		  const Album& album,
		  const QString thumbnailUrl,
		  const int& trackNumber,
		  const QString& spotifyUri,
		  const QString& spotifyWebUrl,
		  const bool& download = true,
		  QObject *parent = nullptr);
	Track(const Track& track);
	~Track();
	
private:
	void init(const QString& name,
			  const QArtistList& artists,
			  const Album& album,
			  const QPixmap& thumbnail,
			  const int& trackNumber,
			  const QString& spotifyUri,
			  const QString& spotifyWebUrl);
	
private slots:
	void thumbnailDownloadFinished(QNetworkReply* reply);
	
public:
	QString getName() const;
	void setName(const QString& value);
	
	QArtistList getArtists() const;
	void setArtists(const QArtistList& value);
	
	Album getAlbum() const;
	void setAlbum(const Album& value);
	
	QPixmap getThumbnail() const;
	void setThumbnail(const QPixmap& value);
	void setThumbnail(const QUrl& thumbnailUrl, const bool& download = true);
	void downloadThumbnail(const QUrl& thumbnailUrl);
	void downloadThumbnail();
	QUrl getThumbnailUrl() const;
	
	int getTrackNumber() const;
	void setTrackNumber(int value);
	
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
	void artistsChanged(QArtistList);
	void albumChanged(Album);
	void thumbnailChanged(QPixmap);
	void trackNumberChanged(int);
	void spotifyUriChanged(QString);
	void spotifyWebUrlChanged(QString);
	
private:
	QString name;
	QArtistList artists;
	QString albumName;
	Album album;
	QPixmap thumbnail;
	int trackNumber;
	QString spotifyUri;
	QString spotifyWebUrl;
	
	QUrl thumbnailUrl;
};

#endif // TRACK_H
