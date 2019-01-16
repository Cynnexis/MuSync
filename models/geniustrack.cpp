#include "geniustrack.h"

GeniusTrack::GeniusTrack(const QString& name,
						 const QArtistList& artists,
						 const QPixmap thumbnail,
						 const QString& lyricsUrl,
						 const QString& apiPath,
						 QObject *parent) : Track(name, artists, thumbnail, parent) {
	init(lyricsUrl,
		 apiPath);
}

GeniusTrack::GeniusTrack(const QString& name,
						 const QArtistList& artists,
						 const QString thumbnailUrl,
						 const QString& lyricsUrl,
						 const QString& apiPath,
						 const bool& download,
						 QObject* parent) : Track(name, artists, thumbnailUrl, download, parent) {
	init(lyricsUrl,
		 apiPath);
}

GeniusTrack::GeniusTrack(const GeniusTrack& track) : Track(track) {
	init(track.getLyricsUrl(),
		 track.getApiPath());
}

GeniusTrack::~GeniusTrack() {}

void GeniusTrack::init(const QString& lyricsUrl,
					   const QString& apiPath) {
	setLyricsUrl(lyricsUrl);
	setApiPath(apiPath);
}

/* GETTERS & SETTERS */

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
			this->getLyricsUrl() == that.getLyricsUrl() &&
			this->getApiPath() == that.getApiPath();
}

bool GeniusTrack::operator!=(const GeniusTrack& that) const {
	return !(*this == that);
}

GeniusTrack& GeniusTrack::operator=(GeniusTrack that) {
	Track::operator=(that);
	init(that.getLyricsUrl(),
		 that.getApiPath());
	return *this;
}
