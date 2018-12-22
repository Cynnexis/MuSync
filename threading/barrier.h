#ifndef BARRIER_H
#define BARRIER_H

#include <iostream>
#include <QObject>
#include <QMutex>
#include <QSharedPointer>

#include "barrierdata.h"

using namespace std;

// Source code inspired from https://stackoverflow.com/a/9639624/7347145

class Barrier : public QObject
{
	Q_OBJECT
public:
	Barrier(int count);
	
	void wait();
	
private:
	QSharedPointer<BarrierData> pBarrierData;
};

#endif // BARRIER_H
