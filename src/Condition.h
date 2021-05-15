#pragma once

#include <assert.h>
#include <pthread.h>
#include "Lock.h"

namespace JINFENG{

class Condition{
public:
	Condition(MutexLock& mutexLock):mutex_(mutexLock.getPthreadMutex())
	{
		int ret = pthread_cond_init(&cond_, NULL);
		assert(ret==0);
	}

	~Condition()
	{
		int ret = pthread_cond_destroy(&cond_);
		assert(ret==0);
	}

	void wait()
	{
		int ret = pthread_cond_wait(&cond_, &mutex_);
		assert(ret==0);
	}

	void notify()
	{
		int ret = pthread_cond_signal(&cond_);
		assert(ret==0);
	}

	void notifyAll()
	{
		int ret = pthread_cond_broadcast(&cond_);
		assert(ret==0);
	}

private:
	pthread_cond_t cond_;
	pthread_mutex_t& mutex_;
};

};
