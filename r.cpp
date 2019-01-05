#include "r.h"

R* R::r;

R::R(QObject *parent) : QObject(parent) {
}

R* R::getInstance(QObject* parent) {
	if (R::r == nullptr)
		R::r = new R(parent);

	return R::r;
}

QString R::getSpotifyClientIdPath() const {
	return ":/text/spotify-client-id";
}

QString R::getSpotifyClientId() const {
	return getCodeFromPath(getSpotifyClientIdPath());
}

QString R::getSpotifyClientSecretPath() const {
	return ":/text/spotify-client-secret";
}

QString R::getSpotifyClientSecret() const {
	return getCodeFromPath(getSpotifyClientSecretPath());
}

QString R::getGeniusClientIdPath() const {
	return ":/text/genius-client-id";
}

QString R::getGeniusClientId() const {
	return getCodeFromPath(getGeniusClientIdPath());
}

QString R::getGeniusClientSecretPath() const {
	return ":/text/genius-client-secret";
}

QString R::getGeniusClientSecret() const {
	return getCodeFromPath(getGeniusClientSecretPath());
}

QColor R::getPrimaryColor() const {
	return QColor(251, 166, 10, 255); // #FBA60A
}

QString R::getMuSyncIconPath() const {
	return ":/svg/musync";
}

QPixmap R::getMuSyncIcon() const {
	return getSvg(getMuSyncIconPath());
}

QString R::getSpotifyPath() const {
	return ":/svg/spotify";
}

QPixmap R::getSpotify() const {
	return getSvg(getSpotifyPath());
}

QString R::getGeniusPath() const {
	return ":/svg/genius";
}

QPixmap R::getGenius() const {
	return getSvg(getGeniusPath());
}

QString R::getGeniusCroppedPath() const {
	return ":/svg/genius-cropped";
}

QPixmap R::getGeniusCropped() const {
	return getSvg(getGeniusCroppedPath());
}

QString R::getQtPath() const {
	return ":/svg/qt";
}

QPixmap R::getQt() const {
	return getSvg(getQtPath());
}

QString R::getRefreshPath() const {
	return ":/svg/refresh";
}

QPixmap R::getRefresh() const {
	return getSvg(getRefreshPath(), getPrimaryColor());
}

QPixmap R::getRefresh(const QColor& tint) {
	return getSvg(getRefreshPath(), tint);
}

QPixmap R::getSvg(const QString& path) const {
	return QPixmap(path);
}

QPixmap R::getSvg(const QString& path, const QColor& tint) const {	
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

QString R::getCodeFromPath(const QString path) const {
	QFile f(path);
	
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		throw "Cannot open the credential files.\n";
	
	QString content = f.readAll().replace("\r\n", "").replace("\r", "").replace("\n", "");
	
	f.close();
	
	return content;
}

void R::setAttrRec(QDomElement& elem, const QString& tagName, const QString& attr, const QString& value) const {
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
