#ifndef MUTEXDATA_H
#define MUTEXDATA_H

#include <iostream>
#include <QObject>
#include <QMutex>

using namespace std;

template <class T>
class MutexData : public QMutex
{	
public:
	MutexData(QMutex::RecursionMode mode = NonRecursive) : QMutex(mode) {}
	MutexData(T& data, QMutex::RecursionMode mode = NonRecursive) : QMutex(mode) {
		setData(data);
	}
	
	~MutexData(){}
	
	void lock() {
		QMutex::lock();
	}
	
	void lock(T& data) {
		setData(data);
		QMutex::lock();
	}
	
	void lockUnlock() {
		QMutex::lock();
		QMutex::unlock();
	}
	
	T lockAndGet() {
		QMutex::lock();
		return getData();
	}
	
	T lockAndGet(T& data) {
		setData(data);
		QMutex::lock();
		return getData();
	}
	
	T lockUnlockAndGet() {
		QMutex::lock();
		QMutex::unlock();
		return getData();
	}
	
	T lockUnlockAndGet(T& data) {
		QMutex::lock(data);
		QMutex::unlock();
		return getData();
	}
	
	/* GETTER & SETTER */
	
	T getData() const {
		return data;
	}
	
	void setData(const T& value) {
		data = value;
	}
	
private:
	T data;
};

#endif // MUTEXDATA_H
