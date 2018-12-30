#ifndef BARRIER_H
#define BARRIER_H

#include <iostream>
#include <QObject>
#include <QMutex>
#include <QSharedPointer>

#include "barrierdata.h"

using namespace std;

// Source code inspired from https://stackoverflow.com/a/9639624/7347145

template <class T>
class Barrier
{
private:
	QSharedPointer<BarrierData<T>> pBarrierData;
	
public:
	Barrier(int count) : pBarrierData(new BarrierData<T>(count)) {}
	Barrier(int count, T& data) : pBarrierData(new BarrierData<T>(count, data)) {}
	
	void wait() {
		pBarrierData->wait();
	}
	
	void wait(const T& data) {
		pBarrierData->wait(data);
	}
	
	T waitAndGet() {
		pBarrierData->waitAndGet();
	}
	
	T waitAndGet(const T& data) {
		return pBarrierData->waitAndGet(data);;
	}
	
	/* GETTER & SETTER */
	
	int getCount() const {
		return pBarrierData->getCount();
	}
	
	void setCount(int value) {
		pBarrierData->setCount(value);
	}
	
	T getData() const {
		return pBarrierData->getData();
	}
	
	void setData(const T& value) {
		pBarrierData->setData(value);
	}
};

#endif // BARRIER_H
