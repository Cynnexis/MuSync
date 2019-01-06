#include "preferences.h"

Preferences* Preferences::pref;

Preferences::Preferences(QObject *parent) : QObject(parent) {
	if (!isFirstLaunch()) {
		// Init
		initStartupBehaviour();
		initCloseButtonMinimized();
		initStyle();
		initRefreshTimeout();
	
		// Last
		initFirstLaunch();
	}
}

Preferences* Preferences::getInstance(QObject* parent) {
	if (Preferences::pref == nullptr)
		Preferences::pref = new Preferences(parent);
	
	return Preferences::pref;
}

bool Preferences::isFirstLaunch() {
	return settings.value("app/firstLaunch", false).toBool();
}

void Preferences::setFirstLaunch(const bool& firstLaunch) {
	settings.setValue("app/firstLaunch", firstLaunch);
	emit firstLaunchChanged(firstLaunch);
}

void Preferences::initFirstLaunch() {
	setFirstLaunch(true);
}

int Preferences::getStartupBehavior() {
	bool ok = false;
	int startupBehaviour = settings.value("app/startupBehaviour", defaultStartupBehaviour()).toInt(&ok);
	
	if (ok && isStartupBehaviourValid(startupBehaviour))
		return startupBehaviour;
	else {
		initStartupBehaviour();
		return getStartupBehavior();
	}
}

void Preferences::setStartupBehaviour(const int& startupBehaviour) {
	if (isStartupBehaviourValid(startupBehaviour)) {
		settings.setValue("app/startupBehaviour", startupBehaviour);
		emit startupBehaviourChanged(startupBehaviour);
	}
}

int Preferences::defaultStartupBehaviour() {
	return DONT_OPEN_AT_STARTUP;
}

void Preferences::initStartupBehaviour() {
	setStartupBehaviour(defaultStartupBehaviour());
}

bool Preferences::isStartupBehaviourValid(const int& startupBehaviour) {
	return startupBehaviour >= 0 && startupBehaviour <= 2;
}

bool Preferences::getCloseButtonMinimized() {
	return settings.value("app/closeButtonMinimized", defaultCloseButtonMinimized()).toBool();
}

void Preferences::setCloseButtonMinimized(const bool& closeButtonMinimized) {
	settings.setValue("app/closeButtonMinimized", closeButtonMinimized);
	emit closeButtonMinimizedChanged(closeButtonMinimized);
}

bool Preferences::defaultCloseButtonMinimized() {
	return false;
}

void Preferences::initCloseButtonMinimized() {
	setCloseButtonMinimized(defaultCloseButtonMinimized());
}

int Preferences::getStyle() {
	bool ok = false;
	int style = settings.value("view/style", defaultStyle()).toInt(&ok);
	
	if (ok && isStyleValid(style))
		return style;
	else {
		initStyle();
		return getStyle();
	}
}

void Preferences::setStyle(const int& style) {
	if (isStyleValid(style)) {
		settings.setValue("view/style", style);
		emit styleChanged(style);
	}
}

int Preferences::defaultStyle() {
	return STYLE_DEFAULT;
}

void Preferences::initStyle() {
	setStyle(defaultStyle());
}

bool Preferences::isStyleValid(const int& style) {
	return 0 <= style && style <= 1;
}

int Preferences::getRefreshTimeout() {
	bool ok = false;
	int refreshTimeout = settings.value("network/refreshTimeout", defaultRefreshTimeout()).toInt(&ok);
	
	if (ok && 1000 <= isRefreshTimeoutValid(refreshTimeout))
		return refreshTimeout;
	else {
		initRefreshTimeout();
		return getRefreshTimeout();
	}
}

void Preferences::setRefreshTimeout(const int& refreshTimeout) {
	if (isRefreshTimeoutValid(refreshTimeout)) {
		settings.setValue("network/refreshTimeout", refreshTimeout);
		emit refreshTimeoutChanged(refreshTimeout);
	}
}

int Preferences::defaultRefreshTimeout() {
	return 5000; // 5s
}

void Preferences::initRefreshTimeout() {
	setRefreshTimeout(defaultRefreshTimeout());
}

bool Preferences::isRefreshTimeoutValid(const int& refreshTimeout) {
	return 1000 <= refreshTimeout && refreshTimeout <= 3600000;
}

void Preferences::clear() {
	settings.clear();
}
