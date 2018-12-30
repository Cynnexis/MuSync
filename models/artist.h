#ifndef ARTIST_H
#define ARTIST_H

#include <iostream>
#include <QObject>

using namespace std;

class Artist : public QObject
{
	Q_OBJECT
public:
	Artist(const QString& name = "",
		   const QString& spotifyUri = "",
		   const QString& spotifyWebUrl = "",
		   QObject *parent = nullptr);
	Artist(const Artist& artist);
	~Artist();
	
private:
	void init(const QString& name,
			  const QString& spotifyUri,
			  const QString& spotifyWebUrl);
	
public:
	/* GETTERS & SETTERS */
	
	QString getName() const;
	void setName(const QString& value);
	
	QString getSpotifyUri() const;
	void setSpotifyUri(const QString& value);
	
	QString getSpotifyWebUrl() const;
	void setSpotifyWebUrl(const QString& value);
	
	QString toString() const;
	
	/* OPERATORS */
	
	static int compare(const Artist& a1, const Artist& a2);
	
	bool operator==(const Artist& that) const;
	bool operator!=(const Artist& that) const;
	bool operator<(const Artist& that) const;
	bool operator>(const Artist& that) const;
	bool operator<=(const Artist& that) const;
	bool operator>=(const Artist& that) const;
	/*QString operator+(const QString& str);
	string operator+(const string& str);*/
	Artist& operator=(Artist that);
	
signals:
	void nameChanged(QString);
	void spotifyUriChanged(QString);
	void spotifyWebUrlChanged(QString);
	
private:
	QString name;
	QString spotifyUri;
	QString spotifyWebUrl;
};

#endif // ARTIST_H
