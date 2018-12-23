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

void Preferences::setFirstLaunch(bool firstLaunch) {
	settings.setValue("app/firstLaunch", firstLaunch);
	emit firstLaunchChanged(firstLaunch);
}

void Preferences::initFirstLaunch() {
	setFirstLaunch(true);
}

int Preferences::getRefreshTimeout() {
	bool ok = false;
	int refreshTimeout = settings.value("network/refreshTimeout", 0).toInt(&ok);
	
	if (ok && refreshTimeout >= 1000) {
		return refreshTimeout;
	}
	else {
		initRefreshTimeout();
		return getRefreshTimeout();
	}
}

void Preferences::setRefreshTimeout(int refreshTimeout) {
	if (refreshTimeout >= 1000) {
		settings.setValue("network/refreshTimeout", refreshTimeout);
		emit refreshTimeoutChanged(refreshTimeout);
	}
}

void Preferences::initRefreshTimeout() {
	setRefreshTimeout(5000); // 5s
}
