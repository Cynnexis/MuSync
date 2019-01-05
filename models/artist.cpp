#include "artist.h"

Artist::Artist(const QString& name,
			   const QString& spotifyUri,
			   const QString& spotifyWebUrl,
			   QObject *parent) : QObject(parent) {
	init(name, spotifyUri, spotifyWebUrl);
}

Artist::Artist(const Artist& artist) {
	init(artist.getName(),
		 artist.getSpotifyUri(),
		 artist.getSpotifyWebUrl());
}

Artist::~Artist() {}

/* GETTERS & SETTERS */

void Artist::init(const QString& name,
				  const QString& spotifyUri,
				  const QString& spotifyWebUrl) {
	setName(name);
	setSpotifyUri(spotifyUri);
	setSpotifyWebUrl(spotifyWebUrl);
}

QString Artist::getName() const {
	return name;
}

void Artist::setName(const QString& value) {
	name = value;
	emit nameChanged(name);
}

QString Artist::getSpotifyUri() const {
	return spotifyUri;
}

void Artist::setSpotifyUri(const QString& value) {
	spotifyUri = value;
	emit spotifyUriChanged(spotifyUri);
}

QString Artist::getSpotifyWebUrl() const {
	return spotifyWebUrl;
}

void Artist::setSpotifyWebUrl(const QString& value) {
	spotifyWebUrl = value;
	emit spotifyWebUrlChanged(spotifyWebUrl);
}

QString Artist::toString() const {
	return getName();
}

/* OPERATORS */

int Artist::compare(const Artist& a1, const Artist& a2) {
	return QString::compare(a1.getName(), a2.getName());
}

bool Artist::operator==(const Artist& that) const {
	return this->getName() == that.getName() &&
			this->getSpotifyUri() == that.getSpotifyUri() &&
			this->getSpotifyWebUrl() == that.getSpotifyWebUrl();
}

bool Artist::operator!=(const Artist& that) const {
	return !(*this == that);
}

bool Artist::operator<(const Artist& that) const {
	return this->getName() < that.getName();
}

bool Artist::operator>(const Artist& that) const {
	return this->getName() > that.getName();
}

bool Artist::operator<=(const Artist& that) const {
	return this->getName() <= that.getName();
}

bool Artist::operator>=(const Artist& that) const {
	return this->getName() >= that.getName();
}

Artist& Artist::operator=(Artist that) {
	init(that.getName(),
		 that.getSpotifyUri(),
		 that.getSpotifyWebUrl());
	return *this;
}

ostream& operator<<(ostream &stream, const Artist &artist) {
	return stream << artist.toString().toStdString();
}
