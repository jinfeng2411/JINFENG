#pragma once

#include "EventLoop.h"
#include "Thread.h"
#include "Lock.h"
#include "Condition.h"
#include <assert.h>

namespace JINFENG{

class EventLoopThread{
public:
	EventLoopThread();
	~EventLoopThread();

	EventLoop* startLoop();

private:
	void threadFunc();

	EventLoop* loop_;
	Thread thread_;
	MutexLock mutex_;
	Condition cond_;
};


};
