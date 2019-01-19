#include "spotifytrack.h"

SpotifyTrack::SpotifyTrack(const QString& name,
						   const QArtistList& artists,
						   const Album& album,
						   const QPixmap thumbnail,
						   const int& trackNumber,
						   const QString& spotifyUri,
						   const QString& spotifyWebUrl,
						   QObject *parent) : Track(name, artists, thumbnail, parent) {
	init(album,
		 trackNumber,
		 spotifyUri,
		 spotifyWebUrl);
}

SpotifyTrack::SpotifyTrack(const QString& name,
						   const QArtistList& artists,
						   const Album& album,
						   const QString thumbnailUrl,
						   const int& trackNumber,
						   const QString& spotifyUri,
						   const QString& spotifyWebUrl,
						   const bool& download,
						   QObject* parent) : Track(name, artists, thumbnailUrl, download, parent) {
	init(album,
		 trackNumber,
		 spotifyUri,
		 spotifyWebUrl);
}

SpotifyTrack::SpotifyTrack(const SpotifyTrack& track) : Track(track) {
	init(track.getAlbum(),
		 track.getTrackNumber(),
		 track.getSpotifyUri(),
		 track.getSpotifyWebUrl());
}

SpotifyTrack::~SpotifyTrack() {}

void SpotifyTrack::init(const Album& album,
				 const int& trackNumber,
				 const QString& spotifyUri,
				 const QString& spotifyWebUrl) {
	setAlbum(album);
	setTrackNumber(trackNumber);
	setSpotifyUri(spotifyUri);
	setSpotifyWebUrl(spotifyWebUrl);
}

/* GETTERS & SETTERS */

Album SpotifyTrack::getAlbum() const {
	return album;
}

void SpotifyTrack::setAlbum(const Album& value) {
	album = value;
	emit albumChanged(album);
}

int SpotifyTrack::getTrackNumber() const {
	return trackNumber;
}

void SpotifyTrack::setTrackNumber(int value) {
	trackNumber = value;
	emit trackNumberChanged(trackNumber);
}

QString SpotifyTrack::getSpotifyUri() const {
	return spotifyUri;
}

void SpotifyTrack::setSpotifyUri(const QString& value) {
	spotifyUri = value;
	emit spotifyUriChanged(spotifyUri);
}

QString SpotifyTrack::getSpotifyWebUrl() const {
	return spotifyWebUrl;
}

void SpotifyTrack::setSpotifyWebUrl(const QString& value) {
	spotifyWebUrl = value;
	emit spotifyWebUrlChanged(spotifyWebUrl);
}

QString SpotifyTrack::toString() const {
	QString repr = Track::toString();
	
	if (getAlbum().getName() != "")
		repr += " [" + getAlbum().getName() + "]";
	
	return repr;
}

bool SpotifyTrack::operator==(const SpotifyTrack& that) const {
	return Track::operator==(that) &&
			this->getAlbum() == that.getAlbum() &&
			this->getTrackNumber() == that.getTrackNumber() &&
			this->getSpotifyUri() == that.getSpotifyUri() &&
			this->getSpotifyWebUrl() == that.getSpotifyWebUrl();
}

bool SpotifyTrack::operator!=(const SpotifyTrack& that) const {
	return !(*this == that);
}

SpotifyTrack& SpotifyTrack::operator=(SpotifyTrack that) {
	Track::operator=(that);
	init(that.getAlbum(),
		 that.getTrackNumber(),
		 that.getSpotifyUri(),
		 that.getSpotifyWebUrl());
	return *this;
}
