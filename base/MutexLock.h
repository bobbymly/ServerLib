#pragma once
#include "noncopyable.h"
#include <pthread.h>
#include <cstdio>

class MutexLock: noncopyable
{
public:
	MutexLock()
	{
		ptread_mutex_init(&mutex,nullptr);
	}
	~MutexLock()
	{
		pthread_mutex_lock(&mutex);
		pthread_mutexdestroy(&mutexx);
	}
	void lock()
	{
		pthread_mutex_lock(&mutex);
	}
	pthread_mutex_t *get()
	{
		return &mutex;
	}

private:
	pthread_mutex_t mutex;
	friend class Condition;
}

class MutexLockGuard: noncopyable
{
explicit MutexLockGuard(MutexLock &_mutex)
		：mutex(_mutex)
	{
		mutex.lock();
	}
	~MutexLockGuard()
	{
		mutex.unlock();
	}
private:
	MutexLock &mutex;
}