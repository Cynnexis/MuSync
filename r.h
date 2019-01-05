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

namespace R
{
	QString getSpotifyClientIdPath();
	QString getSpotifyClientId();
	
	QString getSpotifyClientSecretPath();
	QString getSpotifyClientSecret();
	
	QString getGeniusClientIdPath();
	QString getGeniusClientId();
	
	QString getGeniusClientSecretPath();
	QString getGeniusClientSecret();
	
	QColor getPrimaryColor();
	
	QString getMuSyncIconPath();
	QPixmap getMuSyncIcon();
	
	QString getSpotifyPath();
	QPixmap getSpotify();
	
	QString getGeniusPath();
	QPixmap getGenius();
	
	QString getGeniusCroppedPath();
	QPixmap getGeniusCropped();
	
	QString getQtPath();
	QPixmap getQt();
	
	QString getRefreshPath();
	QPixmap getRefresh();
	QPixmap getRefresh(const QColor& tint);
	
	QString getBrowserPath();
	QPixmap getBrowser();
	QPixmap getBrowser(const QColor& tint);
	
	QString getSettingsPath();
	QPixmap getSettings();
	QPixmap getSettings(const QColor& tint);
	
	QString getPowerPath();
	QPixmap getPower();
	QPixmap getPower(const QColor& tint);
	
	QPixmap getSvg(const QString& path);
	QPixmap getSvg(const QString& path, const QColor& tint);
	
	QString getCodeFromPath(const QString path);
	void setAttrRec(QDomElement& elem, const QString& tagName, const QString& attr, const QString& value);
};

#endif // R_H
