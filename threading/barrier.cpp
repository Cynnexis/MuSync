#include "barrier.h"

Barrier::Barrier(int count) : QObject(nullptr), pBarrierData(new BarrierData(count)) {
}

void Barrier::wait() {
	pBarrierData->wait();
}
