#pragma once
#include "noncopyable.h"
#include "MutexLock.h"
#include "Condition.h"

class CountDownLatch: noncopyable
{
public:
	explicit CountDownLatch(int count);
	void wait();
	void countDown();

private:
	mutable MutexLock mutex_;
	Condition condition_;
	int count_;
}