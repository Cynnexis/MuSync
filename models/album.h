#ifndef ALBUM_H
#define ALBUM_H

#include <iostream>
#include <QObject>
#include <QDate>

using namespace std;

class Album : public QObject
{
	Q_OBJECT
public:
	Album(const QString& name = "",
		  const QDate& releaseDate = QDate(0, 1, 1),
		  const int& totalTracks = 0,
		  const QString& spotifyUri = "",
		  const QString& spotifyWebUrl = "",
		  QObject *parent = nullptr);
	Album(const Album& artist);
	~Album();
	
private:
	void init(const QString& name,
			  const QDate& releaseDate,
			  const int& totalTracks,
			  const QString& spotifyUri,
			  const QString& spotifyWebUrl);
	
public:
	/* GETTERS & SETTERS */
	
	QString getName() const;
	void setName(const QString& value);
	
	QString getSpotifyUri() const;
	void setSpotifyUri(const QString& value);
	
	QString getSpotifyWebUrl() const;
	void setSpotifyWebUrl(const QString& value);
	
	QString toString() const;
	
	/* OPERATORS */
	
	static int compare(const Album& a1, const Album& a2);
	
	bool operator==(const Album& that) const;
	bool operator!=(const Album& that) const;
	bool operator<(const Album& that) const;
	bool operator>(const Album& that) const;
	bool operator<=(const Album& that) const;
	bool operator>=(const Album& that) const;
	Album& operator=(Album that);
	
	QDate getReleaseDate() const;
	void setReleaseDate(const QDate& value);
	
	int getTotalTracks() const;
	void setTotalTracks(int value);
	
signals:
	void nameChanged(QString);
	void releaseDateChanged(QDate);
	void totalTracksChanged(int);
	void spotifyUriChanged(QString);
	void spotifyWebUrlChanged(QString);
	
private:
	QString name;
	QDate releaseDate;
	int totalTracks;
	QString spotifyUri;
	QString spotifyWebUrl;
};

#endif // ALBUM_H
