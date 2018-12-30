#include "qartistlist.h"

QStringList QArtistList::getStringList() const {
	QStringList list;
	for (Artist artist : *this)
		list.append(artist.getName());
	return list;
}

QString QArtistList::join(const QString& sep) const {
	return getStringList().join(sep);
}

QString QArtistList::join(QLatin1String sep) const {
	return getStringList().join(sep);
}

QString QArtistList::join(QChar sep) const {
	return getStringList().join(sep);
}

QString QArtistList::join() const {
	QString result = "";
	
	for (int i = 0, imax = length(); i < imax; i++) {
		result += at(i).getName();
		if (i + 2 < imax)
			result += ", ";
		else if (i + 1 < imax)
			result += " and ";
	}
	
	return result;
}

void QArtistList::sort() {
	qSort(this->begin(), this->end(), Artist::compare);
}
