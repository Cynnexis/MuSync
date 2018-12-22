#ifndef BARRIERDATA_H
#define BARRIERDATA_H

#include <iostream>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

using namespace std;

// Source code inspired from https://stackoverflow.com/a/9639624/7347145

class BarrierData : public QObject
{
	Q_OBJECT
public:
	BarrierData(int count);
	
	void wait();
	
private:
	Q_DISABLE_COPY(BarrierData)
	int count;
	QMutex mutex;
	QWaitCondition waitCondition;
};

#endif // BARRIERDATA_H
