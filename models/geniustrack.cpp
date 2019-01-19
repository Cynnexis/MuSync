#include "geniustrack.h"

GeniusTrack::GeniusTrack(const QString& name,
						 const QArtistList& artists,
						 const QPixmap thumbnail,
						 const QString& lyrics,
						 const QString& lyricsUrl,
						 const QString& apiPath,
						 QObject *parent) : Track(name, artists, thumbnail, parent) {
	init(lyrics,
		 lyricsUrl,
		 apiPath);
}

GeniusTrack::GeniusTrack(const QString& name,
						 const QArtistList& artists,
						 const QString thumbnailUrl,
						 const QString& lyrics,
						 const QString& lyricsUrl,
						 const QString& apiPath,
						 const bool& download,
						 QObject* parent) : Track(name, artists, thumbnailUrl, download, parent) {
	init(lyrics,
		 lyricsUrl,
		 apiPath);
}

GeniusTrack::GeniusTrack(const GeniusTrack& track) : Track(track) {
	init(track.getLyrics(),
		 track.getLyricsUrl(),
		 track.getApiPath());
}

GeniusTrack::~GeniusTrack() {}

void GeniusTrack::init(const QString& lyrics,
					   const QString& lyricsUrl,
					   const QString& apiPath) {
	setLyrics(lyrics);
	setLyricsUrl(lyricsUrl);
	setApiPath(apiPath);
}

/* GETTERS & SETTERS */

QString GeniusTrack::getLyrics() const {
	return lyrics;
}

void GeniusTrack::setLyrics(const QString& value) {
	lyrics = value;
	emit lyricsChanged(lyrics);
}

QString GeniusTrack::getLyricsUrl() const {
	return lyricsUrl;
}

void GeniusTrack::setLyricsUrl(const QString& value) {
	lyricsUrl = value;
	emit lyricsUrlChanged(lyricsUrl);
}

QString GeniusTrack::getApiPath() const {
	return apiPath;
}

void GeniusTrack::setApiPath(const QString& value) {
	apiPath = value;
	emit apiPathChanged(apiPath);
}

bool GeniusTrack::operator==(const GeniusTrack& that) const {
	return Track::operator==(that) &&
			this->getLyrics() == that.getLyrics() &&
			this->getLyricsUrl() == that.getLyricsUrl() &&
			this->getApiPath() == that.getApiPath();
}

bool GeniusTrack::operator!=(const GeniusTrack& that) const {
	return !(*this == that);
}

GeniusTrack& GeniusTrack::operator=(GeniusTrack that) {
	Track::operator=(that);
	init(that.getLyrics(),
		 that.getLyricsUrl(),
		 that.getApiPath());
	return *this;
}
