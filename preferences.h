#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <iostream>
#include <QObject>
#include <QSettings>

using namespace std;

class Preferences : public QObject
{
	Q_OBJECT
public:
	static Preferences* getInstance(QObject* parent = nullptr);
	
	bool isFirstLaunch();
	void setFirstLaunch(bool firstLaunch = true);
	void initFirstLaunch();
	
	int getRefreshTimeout();
	void setRefreshTimeout(int refreshTimeout);
	void initRefreshTimeout();
	
signals:
	void firstLaunchChanged(bool);
	void refreshTimeoutChanged(int);
	
private:
	explicit Preferences(QObject *parent = nullptr);
	
	static Preferences* pref;
	QSettings settings;
};

#endif // PREFERENCES_H
