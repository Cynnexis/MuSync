#include "track.h"

Track::Track(const QString& name, const QStringList& artists, const QString& albumName, const QPixmap thumbnail, QObject *parent) : QObject(parent) {
	init(name, artists, albumName, thumbnail);
}

Track::Track(const QString& name, const QStringList& artists, const QString& albumName, const QString thumbnailUrl, const bool& download, QObject* parent) : QObject(parent) {
	init(name, artists, albumName, QPixmap());
	
	setThumbnail(thumbnailUrl, download);
}

Track::Track(const Track& track) : QObject(track.parent()) {
	init(track.getName(), track.getArtists(), track.getAlbumName(), track.getThumbnail());
	setThumbnail(track.getThumbnailUrl(), false);
}

Track::~Track() {}

void Track::init(const QString& name, const QStringList& artists, const QString& albumName, const QPixmap thumbnail) {
	this->thumbnailUrl = "";
	setName(name);
	setArtists(artists);
	setAlbumName(albumName);
	setThumbnail(thumbnail);
}

void Track::thumbnailDownloadFinished(QNetworkReply* reply) {
	QPixmap thumbnail;
	thumbnail.loadFromData(reply->readAll());
	setThumbnail(thumbnail);
}

/* GETTERS & SETTERS */

QString Track::getName() const {
	return name;
}

void Track::setName(const QString& value) {
	name = value;
	emit nameChanged(name);
}

QStringList Track::getArtists() const {
	return artists;
}

void Track::setArtists(const QStringList& value) {
	artists = value;
	emit artistsChanged(artists);
}

QString Track::getAlbumName() const {
	return albumName;
}

void Track::setAlbumName(const QString& value) {
	albumName = value;
	emit albumNameChanged(albumName);
}

QPixmap Track::getThumbnail() const {
	return thumbnail;
}

void Track::setThumbnail(const QPixmap& value) {
	thumbnail = value;
	emit thumbnailChanged(thumbnail);
}

void Track::setThumbnail(const QUrl& thumbnailUrl, const bool& download) {
	this->thumbnailUrl = thumbnailUrl;
	
	if (download)
		downloadThumbnail();
}

void Track::downloadThumbnail(const QUrl& thumbnailUrl) {
	setThumbnail(thumbnailUrl, true);
}

void Track::downloadThumbnail() {
	if (thumbnailUrl.toString() != "") {
		QNetworkRequest request = QNetworkRequest(QUrl(thumbnailUrl));
		QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
		mgr->get(request);
		
		// Connect to `thumbnailDownloadFinished()`
		connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(thumbnailDownloadFinished(QNetworkReply*)));
	}
}

QUrl Track::getThumbnailUrl() const {
	return this->thumbnailUrl;
}

QString Track::toString() const {
	return "Track{"
		   "name='" + getName() + "', "
		   "artists='" + getArtists().join(", ") + "', "
		   "albumName='" + getAlbumName() + "', "
		   "thumbnailUrl='" + getThumbnailUrl().toString() + "'"
		   "}";
}

bool Track::operator==(const Track& that) const {
	return this->getName() == that.getName() &&
			this->getArtists() == that.getArtists() &&
			this->getAlbumName() == that.getAlbumName() &&
			this->getThumbnailUrl() == that.getThumbnailUrl();
}

bool Track::operator!=(const Track& that) const {
	return !(*this == that);
}

Track& Track::operator=(Track that) {
	init(that.getName(), that.getArtists(), that.getAlbumName(), that.getThumbnail());
	setThumbnail(that.getThumbnailUrl(), false);
	return *this;
}

ostream& operator<<(ostream &stream, const Track &track) {
	return stream << track.toString().toStdString();
}
