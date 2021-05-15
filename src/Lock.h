#pragma once

#include <pthread.h>
#include <assert.h>

namespace JINFENG{

class MutexLock{
public:
	MutexLock()
	{
		int ret = pthread_mutex_init(&mutex_, NULL);
		assert(ret==0);
	}
	~MutexLock()
	{
		int ret = pthread_mutex_destroy(&mutex_);
		assert(ret==0);
	}

	void lock()
	{
		int ret = pthread_mutex_lock(&mutex_);
		assert(ret==0);
	}

	void unlock()
	{
		int ret = pthread_mutex_unlock(&mutex_);
		assert(ret==0);
	}

private:
	pthread_mutex_t mutex_;
};

class MutexLockGuard{
public:
	MutexLockGuard(MutexLock& mutex): mutex_(mutex)
	{
		mutex_.lock();
	}
	~MutexLockGuard()
	{
		mutex_.unlock();
	}
private:
	MutexLock& mutex_;
};


};
