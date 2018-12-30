#include "lyrics.h"

Lyrics::Lyrics(const QString& lyrics,
			   const QString& geniusUrl,
			   QObject *parent) : QObject(parent) {
	init(lyrics, geniusUrl);
}

Lyrics::Lyrics(const Lyrics& lyrics) {
	init(lyrics.getLyrics(), lyrics.getGeniusUrl());
}

Lyrics::~Lyrics() {}

void Lyrics::init(const QString& lyrics, const QString& geniusUrl) {
	setLyrics(lyrics);
	setGeniusUrl(geniusUrl);
}

/* GETTERS & SETTERS */

QString Lyrics::getLyrics() const {
	return lyrics;
}

void Lyrics::setLyrics(const QString& value) {
	lyrics = value;
	emit lyricsChanged(lyrics);
}

QString Lyrics::getGeniusUrl() const {
	return geniusUrl;
}

void Lyrics::setGeniusUrl(const QString& value) {
	geniusUrl = value;
	emit geniusUrlChanged(geniusUrl);
}

QString Lyrics::toString() const {
	return getLyrics();
}

/* OPERATORS */

bool Lyrics::operator==(const Lyrics& that) const {
	return this->getLyrics() == that.getLyrics() &&
			this->getGeniusUrl() == that.getGeniusUrl();
}

bool Lyrics::operator!=(const Lyrics& that) const {
	return !(*this == that);
}

Lyrics& Lyrics::operator=(Lyrics that) {
	init(that.getLyrics(),
		 that.getGeniusUrl());
	return *this;
}

ostream& operator<<(ostream& stream, const Lyrics& lyrics) {
	return stream << lyrics.toString().toStdString();
}
