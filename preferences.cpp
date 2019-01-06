#include "preferences.h"

Preferences* Preferences::pref;

Preferences::Preferences(QObject *parent) : QObject(parent) {
	if (!isFirstLaunch()) {
		// Init
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
	bool success = false;
	int startupBehaviour = settings.value("app/startupBehaviour", DONT_OPEN_AT_STARTUP).toInt(&success);
	
	if (success && startupBehaviour >= 0 && startupBehaviour <= 2)
		return startupBehaviour;
	else {
		initStartupBehaviour();
		return getStartupBehavior();
	}
}

void Preferences::setStartupBehaviour(const int& startupBehaviour) {
	settings.setValue("app/startupBehaviour", startupBehaviour);
	emit startupBehaviourChanged(startupBehaviour);
}

void Preferences::initStartupBehaviour() {
	setStartupBehaviour(DONT_OPEN_AT_STARTUP);
}

bool Preferences::doesCloseButtonMinimized() {
	return settings.value("app/closeButtonMinimized", false).toBool();
}

void Preferences::setCloseButtonMinimized(const bool& closeButtonMinimized) {
	settings.setValue("app/closeButtonMinimized", closeButtonMinimized);
	emit closeButtonMinimizedChanged(closeButtonMinimized);
}

void Preferences::initCloseButtonMinimized() {
	setCloseButtonMinimized(false);
}

int Preferences::getStyle() {
	bool success = false;
	int style = settings.value("view/style", STYLE_DEFAULT).toInt(&success);
	
	if (success && style >= 0 && style <= 1)
		return style;
	else {
		initStyle();
		return getStyle();
	}
}

void Preferences::setStyle(int style) {
	settings.setValue("view/style", style);
	emit styleChanged(style);
}

void Preferences::initStyle() {
	setStyle(STYLE_DEFAULT);
}

int Preferences::getRefreshTimeout() {
	bool ok = false;
	int refreshTimeout = settings.value("network/refreshTimeout", 0).toInt(&ok);
	
	if (ok && refreshTimeout >= 1000)
		return refreshTimeout;
	else {
		initRefreshTimeout();
		return getRefreshTimeout();
	}
}

void Preferences::setRefreshTimeout(const int& refreshTimeout) {
	if (refreshTimeout >= 1000) {
		settings.setValue("network/refreshTimeout", refreshTimeout);
		emit refreshTimeoutChanged(refreshTimeout);
	}
}

void Preferences::initRefreshTimeout() {
	setRefreshTimeout(5000); // 5s
}
