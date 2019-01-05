#include "r.h"

QString R::getSpotifyClientIdPath() {
	return ":/text/spotify-client-id";
}

QString R::getSpotifyClientId() {
	return getCodeFromPath(getSpotifyClientIdPath());
}

QString R::getSpotifyClientSecretPath() {
	return ":/text/spotify-client-secret";
}

QString R::getSpotifyClientSecret() {
	return getCodeFromPath(getSpotifyClientSecretPath());
}

QString R::getGeniusClientIdPath() {
	return ":/text/genius-client-id";
}

QString R::getGeniusClientId() {
	return getCodeFromPath(getGeniusClientIdPath());
}

QString R::getGeniusClientSecretPath() {
	return ":/text/genius-client-secret";
}

QString R::getGeniusClientSecret() {
	return getCodeFromPath(getGeniusClientSecretPath());
}

QColor R::getPrimaryColor() {
	return QColor(251, 166, 10, 255); // #FBA60A
}

QColor R::getSpotifyColor() {
	return QColor(30, 215, 96); // #1ED760
}

QString R::getMuSyncIconPath() {
	return ":/svg/musync";
}

QPixmap R::getMuSyncIcon() {
	return getSvg(getMuSyncIconPath());
}

QString R::getSpotifyPath() {
	return ":/svg/spotify";
}

QPixmap R::getSpotify() {
	return getSvg(getSpotifyPath());
}

QString R::getGeniusPath() {
	return ":/svg/genius";
}

QPixmap R::getGenius() {
	return getSvg(getGeniusPath());
}

QString R::getGeniusCroppedPath() {
	return ":/svg/genius-cropped";
}

QPixmap R::getGeniusCropped() {
	return getSvg(getGeniusCroppedPath());
}

QString R::getQtPath() {
	return ":/svg/qt";
}

QPixmap R::getQt() {
	return getSvg(getQtPath());
}

QString R::getRefreshPath() {
	return ":/svg/refresh";
}

QPixmap R::getRefresh() {
	return getSvg(getRefreshPath(), getPrimaryColor());
}

QPixmap R::getRefresh(const QColor& tint) {
	return getSvg(getRefreshPath(), tint);
}

QString R::getBrowserPath() {
	return ":/svg/browser";
}

QPixmap R::getBrowser() {
	return getSvg(getBrowserPath(), getPrimaryColor());
}

QPixmap R::getBrowser(const QColor& tint) {
	return getSvg(getBrowserPath(), tint);
}

QString R::getSettingsPath() {
	return ":/svg/settings";
}

QPixmap R::getSettings() {
	return getSvg(getSettingsPath(), getPrimaryColor());
}

QPixmap R::getSettings(const QColor& tint) {
	return getSvg(getSettingsPath(), tint);
}

QString R::getPowerPath() {
	return ":/svg/power";
}

QPixmap R::getPower() {
	return getSvg(getPowerPath(), getPrimaryColor());
}

QPixmap R::getPower(const QColor& tint) {
	return getSvg(getPowerPath(), tint);
}

QString R::getUpPath() {
	return ":/svg/up";
}

QPixmap R::getUp() {
	return getSvg(getUpPath(), getPrimaryColor());
}

QPixmap R::getUp(const QColor& tint) {
	return getSvg(getUpPath(), tint);
}

QString R::getDownPath() {
	return ":/svg/down";
}

QPixmap R::getDown() {
	return getSvg(getDownPath(), getPrimaryColor());
}

QPixmap R::getDown(const QColor& tint) {
	return getSvg(getDownPath(), tint);
}

QPixmap R::getSvg(const QString& path) {
	return QPixmap(path);
}

QPixmap R::getSvg(const QString& path, const QColor& tint) {	
	// Source code inspired from https://stackoverflow.com/questions/15123544/change-the-color-of-an-svg-in-qt
	QFile f(path);
	
	if (!f.open(QIODevice::ReadOnly))
		throw "Cannot open " + path.toStdString();
	
	QByteArray data = f.readAll();
	
	QDomDocument doc;
	doc.setContent(data);
	QDomElement docElem = doc.documentElement();
	setAttrRec(docElem, "path", "fill", tint.name());
	
	QSvgRenderer renderer(doc.toByteArray());
	QPixmap pixmap(renderer.defaultSize());
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	renderer.render(&painter);
	return pixmap;
}

QString R::getCodeFromPath(const QString path) {
	QFile f(path);
	
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		throw "Cannot open the credential files.\n";
	
	QString content = f.readAll().replace("\r\n", "").replace("\r", "").replace("\n", "");
	
	f.close();
	
	return content;
}

void R::setAttrRec(QDomElement& elem, const QString& tagName, const QString& attr, const QString& value) {
	if (elem.tagName().compare(tagName) == 0 &&
		(attr != "fill" || (attr == "fill" && elem.attribute(attr, "") != "none"))) {
		elem.setAttribute(attr, value);
	}
	
	for (int i = 0, imax = elem.childNodes().count(); i < imax; i++) {
		QDomNode child = elem.childNodes().at(i);
		
		if (!child.isElement())
			continue;
		
		QDomElement childElem = child.toElement();
		setAttrRec(childElem, tagName, attr, value);
	}
}
