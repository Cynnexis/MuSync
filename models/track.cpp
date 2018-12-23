#include "track.h"

Track::Track(const QString& name, const QStringList& artists, const QString& albumName, const QPixmap thumbnail, QObject *parent) : QObject(parent) {
	init(name, artists, albumName, thumbnail);
}

Track::Track(const QString& name, const QStringList& artists, const QString& albumName, const QString thumbnailUrl, QObject* parent) : QObject(parent) {
	if (thumbnailUrl == "")
		init(name, artists, albumName, QPixmap());
	// Download image
	else {
		QNetworkRequest request = QNetworkRequest(QUrl(thumbnailUrl));
		QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
		QNetworkReply* response = mgr->get(request);
		QEventLoop event;
		connect(response, SIGNAL(finished()), &event, SLOT(quit()));	
		event.exec();
		
		QPixmap thumbnail;
		thumbnail.loadFromData(response->readAll());
		init(name, artists, albumName, thumbnail);
	}
}

Track::Track(const Track& track, QObject *parent) : QObject(parent) {
	init(track.getName(), track.getArtists(), track.getAlbumName(), track.getThumbnail());
}

void Track::init(const QString& name, const QStringList& artists, const QString& albumName, const QPixmap thumbnail){
	setName(name);
	setArtists(artists);
	setAlbumName(albumName);
	setThumbnail(thumbnail);
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

QPixmap Track::getThumbnail() const {
	return thumbnail;
}

void Track::setThumbnail(const QPixmap& value) {
	thumbnail = value;
}

QString Track::toString() const {
	return "Track{"
		   "name='" + getName() + "', "
		   "artists='" + getArtists().join(", ") + "', "
		   "albumName='" + getAlbumName() + "'"
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
