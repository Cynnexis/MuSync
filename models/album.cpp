#include "album.h"

Album::Album(const QString& name,
			 const QDate& releaseDate,
			 const int& totalTracks,
			 const QString& spotifyUri,
			 const QString& spotifyWebUrl,
			 QObject *parent) : QObject(parent) {
	init(name, releaseDate, totalTracks, spotifyUri, spotifyWebUrl);
}

Album::Album(const Album& album) {
	init(album.getName(),
		 album.getReleaseDate(),
		 album.getTotalTracks(),
		 album.getSpotifyUri(),
		 album.getSpotifyWebUrl());
}

Album::~Album() {}

/* GETTERS & SETTERS */

void Album::init(const QString& name,
				 const QDate& releaseDate,
				 const int& totalTracks,
				  const QString& spotifyUri,
				  const QString& spotifyWebUrl) {
	setName(name);
	setReleaseDate(releaseDate);
	setTotalTracks(totalTracks);
	setSpotifyUri(spotifyUri);
	setSpotifyWebUrl(spotifyWebUrl);
}

QString Album::getName() const {
	return name;
}

void Album::setName(const QString& value) {
	name = value;
	emit nameChanged(name);
}

QDate Album::getReleaseDate() const {
	return releaseDate;
}

void Album::setReleaseDate(const QDate& value) {
	releaseDate = value;
	emit releaseDateChanged(releaseDate);
}

int Album::getTotalTracks() const {
	return totalTracks;
}

void Album::setTotalTracks(int value) {
	totalTracks = value;
	emit totalTracksChanged(totalTracks);
}

QString Album::getSpotifyUri() const {
	return spotifyUri;
}

void Album::setSpotifyUri(const QString& value) {
	spotifyUri = value;
	emit spotifyUriChanged(spotifyUri);
}

QString Album::getSpotifyWebUrl() const {
	return spotifyWebUrl;
}

void Album::setSpotifyWebUrl(const QString& value) {
	spotifyWebUrl = value;
	emit spotifyWebUrlChanged(spotifyWebUrl);
}

QString Album::toString() const {
	/*return "Album{"
			"name='" + getName() + "', "
			"releaseDate='" + getReleaseDate().toString() + "', "
			"totalTracks=" + QString::number(getTotalTracks()) + ", "
			"spotifyUri='" + getSpotifyUri() + "', "
			"spotifyWebUrl='" + getSpotifyWebUrl() + "'"
			"}";*/
	return getName();
}

/* OPERATORS */

int Album::compare(const Album& a1, const Album& a2) {
	return QString::compare(a1.getName(), a2.getName());
}

bool Album::operator==(const Album& that) const {
	return this->getName() == that.getName() &&
			this->getReleaseDate() == that.getReleaseDate() &&
			this->getTotalTracks() == that.getTotalTracks() &&
			this->getSpotifyUri() == that.getSpotifyUri() &&
			this->getSpotifyWebUrl() == that.getSpotifyWebUrl();
}

bool Album::operator!=(const Album& that) const {
	return !(*this == that);
}

bool Album::operator<(const Album& that) const {
	return this->getName() < that.getName();
}

bool Album::operator>(const Album& that) const {
	return this->getName() > that.getName();
}

bool Album::operator<=(const Album& that) const {
	return this->getName() <= that.getName();
}

bool Album::operator>=(const Album& that) const {
	return this->getName() >= that.getName();
}

Album& Album::operator=(Album that) {
	init(that.getName(),
		 that.getReleaseDate(),
		 that.getTotalTracks(),
		 that.getSpotifyUri(),
		 that.getSpotifyWebUrl());
	return *this;
}

ostream& operator<<(ostream &stream, const Album& album) {
	return stream << album.toString().toStdString();
}
