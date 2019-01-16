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
				const QString& lyricsUrl = "",
				const QString& apiPath = "",
				QObject *parent = nullptr);
	GeniusTrack(const QString& name,
				const QArtistList& artists,
				const QString thumbnailUrl,
				const QString& lyricsUrl,
				const QString& apiPath,
				const bool& download = true,
				QObject *parent = nullptr);
	GeniusTrack(const GeniusTrack& track);
	~GeniusTrack();
	
private:
	void init(const QString& lyricsUrl,
			  const QString& apiPath);
	
public:
	QString getLyricsUrl() const;
	void setLyricsUrl(const QString& value);
	
	QString getApiPath() const;
	void setApiPath(const QString& value);
	
	bool operator==(const GeniusTrack& that) const;
	bool operator!=(const GeniusTrack& that) const;
	GeniusTrack& operator=(GeniusTrack that);
	
signals:
	void lyricsUrlChanged(QString);
	void apiPathChanged(QString);
	
private:
	QString lyricsUrl;
	QString apiPath;
};

#endif // GENIUSTRACK_H
