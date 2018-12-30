#include "track.h"

Track::Track(const QString& name,
			 const QArtistList& artists,
			 const QString& albumName,
			 const QPixmap thumbnail,
			 const QString& spotifyUri,
			 const QString& spotifyWebUrl,
			 QObject *parent) : QObject(parent) {
	init(name, artists, albumName, thumbnail, spotifyUri, spotifyWebUrl);
}

Track::Track(const QString& name,
			 const QArtistList& artists,
			 const QString& albumName,
			 const QString thumbnailUrl,
			 const QString& spotifyUri,
			 const QString& spotifyWebUrl,
			 const bool& download,
			 QObject* parent) : QObject(parent) {
	init(name, artists, albumName, QPixmap(), spotifyUri, spotifyWebUrl);
	
	setThumbnail(thumbnailUrl, download);
}

Track::Track(const Track& track) : QObject(track.parent()) {
	init(track.getName(),
		 track.getArtists(),
		 track.getAlbumName(),
		 track.getThumbnail(),
		 track.getSpotifyUri(),
		 track.getSpotifyWebUrl());
	setThumbnail(track.getThumbnailUrl(), false);
}

Track::~Track() {}

void Track::init(const QString& name,
				 const QArtistList& artists,
				 const QString& albumName,
				 const QPixmap thumbnail,
				 const QString spotifyUri,
				 const QString spotifyWebUrl) {
	this->thumbnailUrl = "";
	setName(name);
	setArtists(artists);
	setAlbumName(albumName);
	setThumbnail(thumbnail);
	setSpotifyUri(spotifyUri);
	setSpotifyWebUrl(spotifyWebUrl);
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

QString Track::getSpotifyUri() const {
	return spotifyUri;
}

void Track::setSpotifyUri(const QString& value) {
	spotifyUri = value;
	emit spotifyUriChanged(spotifyUri);
}

QString Track::getSpotifyWebUrl() const {
	return spotifyWebUrl;
}

void Track::setSpotifyWebUrl(const QString& value) {
	spotifyWebUrl = value;
	emit spotifyWebUrlChanged(spotifyWebUrl);
}

QString Track::toString() const {
	return "Track{"
		   "name='" + getName() + "', "
		   "artists='" + getArtists().join(", ") + "', "
		   "albumName='" + getAlbumName() + "', "
		   "thumbnailUrl='" + getThumbnailUrl().toString() + "', "
		   "spotifyUri='" + getSpotifyUri() + "', "
		   "spotifyWebUrl='" + getSpotifyWebUrl() + "'"
		   "}";
}

bool Track::operator==(const Track& that) const {
	return this->getName() == that.getName() &&
			this->getArtists() == that.getArtists() &&
			this->getAlbumName() == that.getAlbumName() &&
			this->getThumbnailUrl() == that.getThumbnailUrl() &&
			this->getSpotifyUri() == that.getSpotifyUri() &&
			this->getSpotifyWebUrl() == that.getSpotifyWebUrl();
}

bool Track::operator!=(const Track& that) const {
	return !(*this == that);
}

Track& Track::operator=(Track that) {
	init(that.getName(),
		 that.getArtists(),
		 that.getAlbumName(),
		 that.getThumbnail(),
		 that.getSpotifyUri(),
		 that.getSpotifyWebUrl());
	setThumbnail(that.getThumbnailUrl(), false);
	return *this;
}

ostream& operator<<(ostream &stream, const Track &track) {
	return stream << track.toString().toStdString();
}
