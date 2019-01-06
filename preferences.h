#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <iostream>
#include <QObject>
#include <QSettings>
#include <QApplication>


using namespace std;

class Preferences : public QObject
{
	Q_OBJECT
public:
	static Preferences* getInstance(QObject* parent = nullptr);
	
	bool isFirstLaunch();
	void setFirstLaunch(const bool& firstLaunch = true);
	bool defaultFirstLaunch();
	void initFirstLaunch();
	
	int getStartupBehavior();
	void setStartupBehaviour(const int& startupBehaviour);
	int defaultStartupBehaviour();
	void initStartupBehaviour();
	bool isStartupBehaviourValid(const int& startupBehaviour);
	
	bool getCloseButtonMinimized();
	void setCloseButtonMinimized(const bool& closeButtonMinimized);
	bool defaultCloseButtonMinimized();
	void initCloseButtonMinimized();
	
	int getStyle();
	void setStyle(const int& style);
	int defaultStyle();
	void initStyle();
	bool isStyleValid(const int& style);
	
	int getRefreshTimeout();
	void setRefreshTimeout(const int& refreshTimeout);
	int defaultRefreshTimeout();
	void initRefreshTimeout();
	bool isRefreshTimeoutValid(const int& refreshTimeout);
	
	void runAppAtStartup(bool runAtStartup);
	
	void clear();
	
	static const int DONT_OPEN_AT_STARTUP = 0;
	static const int OPEN_AT_STARTUP = 1;
	static const int OPEN_AT_STARTUP_MINIMIZED = 2;
	
	static const int STYLE_DEFAULT = 0;
	static const int STYLE_DARK = 1;
	
signals:
	void firstLaunchChanged(bool);
	void startupBehaviourChanged(int);
	void closeButtonMinimizedChanged(bool);
	void styleChanged(int);
	void refreshTimeoutChanged(int);
	
private:
	explicit Preferences(QObject *parent = nullptr);
	
	static Preferences* pref;
	QSettings settings;
};

#endif // PREFERENCES_H
