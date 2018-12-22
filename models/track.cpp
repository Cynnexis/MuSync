#include "track.h"

Track::Track(const QString& name, const QStringList& artists, const QString& albumName, QObject *parent) : QObject(parent) {
	init(name, artists, albumName);
}

Track::Track(const Track& track, QObject *parent) : QObject(parent) {
	init(track.getName(), track.getArtists(), track.getAlbumName());
}

void Track::init(const QString& name, const QStringList& artists, const QString& albumName){
	setName(name);
	setArtists(artists);
	setAlbumName(albumName);
}

/* GETTERS & SETTERS */

QString Track::getName() const {
	return name;
}

void Track::setName(const QString& value) {
	name = value;
}

QStringList Track::getArtists() const {
	return artists;
}

void Track::setArtists(const QStringList& value) {
	artists = value;
}

QString Track::getAlbumName() const {
	return albumName;
}

void Track::setAlbumName(const QString& value) {
	albumName = value;
}

QString Track::toString() const {
	return "Track{"
		   "name='" + getName() + "', "
		   "artists='" + getArtists().join(", ") + "', "
		   "albumName='" + getAlbumName() + "', "
		   "}";
}

bool Track::operator==(const Track& that) const {
	return this->getName() == that.getName() &&
			this->getArtists() == that.getArtists() &&
			this->getAlbumName() == that.getAlbumName();
}

bool Track::operator!=(const Track& that) const {
	return !(*this == that);
}

ostream& operator<<(ostream &stream, const Track &track) {
	return stream << track.toString().toStdString();
}
