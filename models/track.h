#ifndef TRACK_H
#define TRACK_H

#include <iostream>
#include <QObject>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

using namespace std;

class Track : public QObject
{
	Q_OBJECT
public:
	Track(const QString& name = "", const QStringList& artists = {}, const QString& albumName = "", const QPixmap thumbnail = QPixmap(), QObject *parent = nullptr);
	Track(const QString& name, const QStringList& artists, const QString& albumName, const QString thumbnailUrl, QObject *parent = nullptr);
	Track(const Track& track, QObject *parent = nullptr);
	
private:
	void init(const QString& name, const QStringList& artists, const QString& albumName, const QPixmap thumbnail);
	
public:
	QString getName() const;
	void setName(const QString& value);
	
	QStringList getArtists() const;
	void setArtists(const QStringList& value);
	
	QString getAlbumName() const;
	void setAlbumName(const QString& value);
	
	QPixmap getThumbnail() const;
	void setThumbnail(const QPixmap& value);
	
	QString toString() const;
	
	bool operator==(const Track& that) const;
	bool operator!=(const Track& that) const;
	
private:
	QString name;
	QStringList artists;
	QString albumName;
	QPixmap thumbnail;
};

#endif // TRACK_H
