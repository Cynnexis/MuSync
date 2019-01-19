#ifndef GENIUSTRACK_H
#define GENIUSTRACK_H

#include <iostream>

#include "track.h"

using namespace std;

class GeniusTrack : public Track
{
	Q_OBJECT
	
public:
	GeniusTrack(const QString& name = "",
				const QArtistList& artists = {},
				const QPixmap thumbnail = QPixmap(),
				const QString& lyrics = "",
				const QString& lyricsUrl = "",
				const QString& apiPath = "",
				QObject *parent = nullptr);
	GeniusTrack(const QString& name,
				const QArtistList& artists,
				const QString thumbnailUrl,
				const QString& lyrics,
				const QString& lyricsUrl,
				const QString& apiPath,
				const bool& download = true,
				QObject *parent = nullptr);
	GeniusTrack(const GeniusTrack& track);
	~GeniusTrack();
	
private:
	void init(const QString& lyrics,
			  const QString& lyricsUrl,
			  const QString& apiPath);
	
public:
	
	/* GETTERS & SETTERS */
	
	QString getLyrics() const;
	void setLyrics(const QString& value);
	
	QString getGeniusUrl() const;
	void setGeniusUrl(const QString& value);
	
	QString getLyricsUrl() const;
	void setLyricsUrl(const QString& value);
	
	QString getApiPath() const;
	void setApiPath(const QString& value);
	
	bool operator==(const GeniusTrack& that) const;
	bool operator!=(const GeniusTrack& that) const;
	GeniusTrack& operator=(GeniusTrack that);
	
signals:
	void lyricsChanged(QString);
	void lyricsUrlChanged(QString);
	void apiPathChanged(QString);
	
private:
	QString lyrics;
	QString lyricsUrl;
	QString apiPath;
};

#endif // GENIUSTRACK_H
