#ifndef BARRIERDATA_H
#define BARRIERDATA_H

#include <iostream>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

#include "mutexdata.h"

using namespace std;

// Source code inspired from https://stackoverflow.com/a/9639624/7347145

template <class T>
class BarrierData
{
public:
	BarrierData(int count) : count(count) {}
	BarrierData(int count, T& data) : count(count) {
		mutex.setData(data);
	}
	
	void wait() {
		mutex.lock();
		
		if (--count > 0)
			waitCondition.wait(&mutex);
		else
			waitCondition.wakeAll();
		mutex.unlock();
	}
	
	void wait(const T& data) {
		mutex.setData(data);
		wait();
	}
	
	T waitAndGet() {
		wait();
		return mutex.getData();
	}
	
	T waitAndGet(const T& data) {
		wait(data);
		return mutex.getData();
	}
	
	/* GETTER & SETTER */
	
	int getCount() const {
		return count;
	}
	
	void setCount(int value) {
		count = value;
	}
	
	T getData() const {
		return mutex.getData();
	}
	
	void setData(const T& value) {
		mutex.setData(value);
	}
	
private:
	Q_DISABLE_COPY(BarrierData<T>)
	int count;
	MutexData<T> mutex;
	QWaitCondition waitCondition;
};

#endif // BARRIERDATA_H
