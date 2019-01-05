#ifndef LYRICS_H
#define LYRICS_H

#include <iostream>
#include <QObject>

using namespace std;

class Lyrics : public QObject
{
	Q_OBJECT
public:
	Lyrics(const QString& lyrics = "",
		   const QString& geniusUrl = "",
		   QObject *parent = nullptr);
	Lyrics(const Lyrics& lyrics);
	~Lyrics();
	
private:
	void init(const QString& lyrics = "",
			  const QString& geniusUrl = "");
	
public:
	
	/* GETTERS & SETTERS */
	
	QString getLyrics() const;
	void setLyrics(const QString& value);
	
	QString getGeniusUrl() const;
	void setGeniusUrl(const QString& value);
	
	QString toString() const;
	
	/* OPERATORS */
	
	bool operator==(const Lyrics& that) const;
	bool operator!=(const Lyrics& that) const;
	Lyrics& operator=(Lyrics that);
	
signals:
	void lyricsChanged(QString);
	void geniusUrlChanged(QString);
	
private:
	QString lyrics;
	QString geniusUrl;
};

#endif // LYRICS_H
