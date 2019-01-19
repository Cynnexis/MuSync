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
		  const QPixmap thumbnail = QPixmap(),
		  QObject *parent = nullptr);
	Track(const QString& name,
		  const QArtistList& artists,
		  const QString thumbnailUrl,
		  const bool& download = true,
		  QObject *parent = nullptr);
	Track(const Track& track);
	~Track();
	
private:
	void init(const QString& name,
			  const QArtistList& artists,
			  const QPixmap& thumbnail);
	
private slots:
	void thumbnailDownloadFinished(QNetworkReply* reply);
	
public:
	QString getName() const;
	void setName(const QString& value);
	
	QArtistList getArtists() const;
	void setArtists(const QArtistList& value);
	
	QPixmap getThumbnail() const;
	void setThumbnail(const QPixmap& value);
	void setThumbnail(const QUrl& thumbnailUrl, const bool& download = true);
	void downloadThumbnail(const QUrl& thumbnailUrl);
	void downloadThumbnail();
	QUrl getThumbnailUrl() const;
	
	QString toString() const;
	
	bool operator==(const Track& that) const;
	bool operator!=(const Track& that) const;
	Track& operator=(Track that);
	
signals:
	void nameChanged(QString);
	void artistsChanged(QArtistList);
	void thumbnailChanged(QPixmap);
	
private:
	QString name;
	QArtistList artists;
	QPixmap thumbnail;
	
	QUrl thumbnailUrl;
};

#endif // TRACK_H
