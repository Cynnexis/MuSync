#include "track.h"

Track::Track(const QString& name,
			 const QArtistList& artists,
			 const QPixmap thumbnail,
			 QObject *parent) : QObject(parent) {
	init(name,
		 artists,
		 thumbnail);
}

Track::Track(const QString& name,
			 const QArtistList& artists,
			 const QString thumbnailUrl,
			 const bool& download,
			 QObject* parent) : QObject(parent) {
	init(name,
		 artists,
		 QPixmap());
	
	setThumbnail(thumbnailUrl, download);
}

Track::Track(const Track& track) : QObject(track.parent()) {
	init(track.getName(),
		 track.getArtists(),
		 track.getThumbnail());
	setThumbnail(track.getThumbnailUrl(), false);
}

Track::~Track() {}

void Track::init(const QString& name,
				 const QArtistList& artists,
				 const QPixmap& thumbnail) {
	this->thumbnailUrl = "";
	setName(name);
	setArtists(artists);
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

QArtistList Track::getArtists() const {
	return artists;
}

void Track::setArtists(const QArtistList& value) {
	artists = value;
	emit artistsChanged(artists);
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
	/*return "Track{"
		   "name='" + getName() + "', "
		   "artists='" + getArtists().join(", ") + "', "
		   "album='" + getAlbum().getName() + "', "
		   "thumbnailUrl='" + getThumbnailUrl().toString() + "', "
		   "trackNumber=" + QString::number(getTrackNumber()) + ", "
		   "spotifyUri='" + getSpotifyUri() + "', "
		   "spotifyWebUrl='" + getSpotifyWebUrl() + "'"
		   "}";*/
	QString repr = getName();
	QString rArtists = getArtists().join();
	
	if (rArtists != "")
		repr += " by " + rArtists;
	
	return repr;
}

bool Track::operator==(const Track& that) const {
	return this->getName() == that.getName() &&
			this->getArtists() == that.getArtists() &&
			this->getThumbnailUrl() == that.getThumbnailUrl();
}

bool Track::operator!=(const Track& that) const {
	return !(*this == that);
}

Track& Track::operator=(Track that) {
	init(that.getName(),
		 that.getArtists(),
		 that.getThumbnail());
	setThumbnail(that.getThumbnailUrl(), false);
	return *this;
}
