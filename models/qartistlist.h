#ifndef QARTISTLIST_H
#define QARTISTLIST_H

#include <iostream>
#include <QObject>
#include <QList>

#include "artist.h"

using namespace std;

class QArtistList : public QList<Artist>
{
public:
	QStringList getStringList() const;
	
	QString join(const QString& sep) const;
	QString join(QLatin1String sep) const;
	QString join(QChar sep) const;
	QString join() const;
	
	void sort();
};

#endif // QARTISTLIST_H
