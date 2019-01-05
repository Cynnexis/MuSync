#ifndef R_H
#define R_H

#include <iostream>
#include <QObject>
#include <QFile>
#include <QPixmap>
#include <QDomElement>
#include <QSvgRenderer>
#include <QPainter>
#include <QIcon>
#include <QTextStream>

using namespace std;

class R : public QObject
{
	Q_OBJECT
public:
	static R* getInstance(QObject* parent = nullptr);
	
	QString getSpotifyClientIdPath() const;
	QString getSpotifyClientId() const;
	
	QString getSpotifyClientSecretPath() const;
	QString getSpotifyClientSecret() const;
	
	QString getGeniusClientIdPath() const;
	QString getGeniusClientId() const;
	
	QString getGeniusClientSecretPath() const;
	QString getGeniusClientSecret() const;
	
	QColor getPrimaryColor() const;
	
	QString getMuSyncIconPath() const;
	QPixmap getMuSyncIcon() const;
	
	QString getSpotifyPath() const;
	QPixmap getSpotify() const;
	
	QString getGeniusPath() const;
	QPixmap getGenius() const;
	
	QString getGeniusCroppedPath() const;
	QPixmap getGeniusCropped() const;
	
	QString getQtPath() const;
	QPixmap getQt() const;
	
	QString getRefreshPath() const;
	QPixmap getRefresh() const;
	QPixmap getRefresh(const QColor& tint);
	
	QPixmap getSvg(const QString& path) const;
	QPixmap getSvg(const QString& path, const QColor& tint) const;
	
private:
	explicit R(QObject *parent = nullptr);
	
	static R* r;
	
	QString getCodeFromPath(const QString path) const;
	void setAttrRec(QDomElement& elem, const QString& tagName, const QString& attr, const QString& value) const;
};

#endif // R_H
