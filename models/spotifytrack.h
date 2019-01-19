#ifndef SPOTIFYTRACK_H
#define SPOTIFYTRACK_H

#include <iostream>

#include "track.h"

using namespace std;

class SpotifyTrack : public Track
{
	Q_OBJECT
	
public:
	SpotifyTrack(const QString& name = "",
				 const QArtistList& artists = {},
				 const Album& album = Album(),
				 const QPixmap thumbnail = QPixmap(),
				 const int& trackNumber = 0,
				 const QString& spotifyUri = "",
				 const QString& spotifyWebUrl = "",
				 QObject *parent = nullptr);
	SpotifyTrack(const QString& name,
				 const QArtistList& artists,
				 const Album& album,
				 const QString thumbnailUrl,
				 const int& trackNumber,
				 const QString& spotifyUri,
				 const QString& spotifyWebUrl,
				 const bool& download = true,
				 QObject *parent = nullptr);
	SpotifyTrack(const SpotifyTrack& track);
	~SpotifyTrack();
	
private:
	void init(const Album& album,
			  const int& trackNumber,
			  const QString& spotifyUri,
			  const QString& spotifyWebUrl);
	
public:
	Album getAlbum() const;
	void setAlbum(const Album& value);
	
	int getTrackNumber() const;
	void setTrackNumber(int value);
	
	QString getSpotifyUri() const;
	void setSpotifyUri(const QString& value);
	
	QString getSpotifyWebUrl() const;
	void setSpotifyWebUrl(const QString& value);
	
	QString toString() const;
	
	bool operator==(const SpotifyTrack& that) const;
	bool operator!=(const SpotifyTrack& that) const;
	SpotifyTrack& operator=(SpotifyTrack that);
	
signals:
	void albumChanged(Album);
	void trackNumberChanged(int);
	void spotifyUriChanged(QString);
	void spotifyWebUrlChanged(QString);
	
private:
	Album album;
	int trackNumber;
	QString spotifyUri;
	QString spotifyWebUrl;
};

#endif // SPOTIFYTRACK_H
