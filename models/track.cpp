#include "track.h"

Track::Track(const QString& name,
			 const QArtistList& artists,
			 const Album& album,
			 const QPixmap thumbnail,
			 const int& trackNumber,
			 const QString& spotifyUri,
			 const QString& spotifyWebUrl,
			 QObject *parent) : QObject(parent) {
	init(name,
		 artists,
		 album,
		 thumbnail,
		 trackNumber,
		 spotifyUri,
		 spotifyWebUrl);
}

Track::Track(const QString& name,
			 const QArtistList& artists,
			 const Album& album,
			 const QString thumbnailUrl, const int& trackNumber,
			 const QString& spotifyUri,
			 const QString& spotifyWebUrl,
			 const bool& download,
			 QObject* parent) : QObject(parent) {
	init(name,
		 artists,
		 album,
		 QPixmap(),
		 trackNumber,
		 spotifyUri,
		 spotifyWebUrl);
	
	setThumbnail(thumbnailUrl, download);
}

Track::Track(const Track& track) : QObject(track.parent()) {
	init(track.getName(),
		 track.getArtists(),
		 track.getAlbum(),
		 track.getThumbnail(),
		 track.getTrackNumber(),
		 track.getSpotifyUri(),
		 track.getSpotifyWebUrl());
	setThumbnail(track.getThumbnailUrl(), false);
}

Track::~Track() {}

void Track::init(const QString& name,
				 const QArtistList& artists,
				 const Album& album,
				 const QPixmap& thumbnail,
				 const int& trackNumber,
				 const QString& spotifyUri,
				 const QString& spotifyWebUrl) {
	this->thumbnailUrl = "";
	setName(name);
	setArtists(artists);
	setAlbum(album);
	setThumbnail(thumbnail);
	setTrackNumber(trackNumber);
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

Album Track::getAlbum() const {
	return album;
}

void Track::setAlbum(const Album& value) {
	album = value;
	emit albumChanged(album);
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

int Track::getTrackNumber() const {
	return trackNumber;
}

void Track::setTrackNumber(int value) {
	trackNumber = value;
	emit trackNumberChanged(trackNumber);
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
	
	if (getAlbum().getName() != "")
		repr += " [" + getAlbum().getName() + "]";
	
	return repr;
}

bool Track::operator==(const Track& that) const {
	return this->getName() == that.getName() &&
			this->getArtists() == that.getArtists() &&
			this->getAlbum() == that.getAlbum() &&
			this->getThumbnailUrl() == that.getThumbnailUrl() &&
			this->getTrackNumber() == that.getTrackNumber() &&
			this->getSpotifyUri() == that.getSpotifyUri() &&
			this->getSpotifyWebUrl() == that.getSpotifyWebUrl();
}

bool Track::operator!=(const Track& that) const {
	return !(*this == that);
}

Track& Track::operator=(Track that) {
	init(that.getName(),
		 that.getArtists(),
		 that.getAlbum(),
		 that.getThumbnail(),
		 that.getTrackNumber(),
		 that.getSpotifyUri(),
		 that.getSpotifyWebUrl());
	setThumbnail(that.getThumbnailUrl(), false);
	return *this;
}
