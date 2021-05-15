#pragma once

#include <pthread.h>
#include <functional>
#include <assert.h>

namespace JINFENG{
class Thread{
public:
	typedef std::function<void()> ThreadFunc;
	Thread(std::function<void()> func):func_(func)
	{
		
	}

	~Thread()
	{

	}
	void start()
	{
		int ret = pthread_create(&tid, NULL, Thread::run, this);
		assert(ret==0);
	}

	void join()
	{
		int ret = pthread_join(tid, NULL);
		assert(ret==0);
	}

	
private:
	static void* run(void *arg)
	{
		Thread* t = (Thread*)arg;
		t->func_();
	}
private:
	pthread_t tid;
	ThreadFunc func_;
};

};
