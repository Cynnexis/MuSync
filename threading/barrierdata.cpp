#include "barrierdata.h"

BarrierData::BarrierData(int count) : QObject(nullptr), count(count) {
}

void BarrierData::wait() {
	mutex.lock();
	
	if (--count > 0)
		waitCondition.wait(&mutex);
	else
		waitCondition.wakeAll();
	mutex.unlock();
}
