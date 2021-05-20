#include "EventLoopThread.h"

namespace JINFENG{

EventLoopThread::EventLoopThread()
	:loop_(nullptr),
	thread_(std::bind(&EventLoopThread::threadFunc, this)),
	mutex_(),
	cond_(mutex_)
{

}

EventLoopThread::~EventLoopThread()
{

}

EventLoop* EventLoopThread::startLoop()
{
	assert(loop_==nullptr);
	thread_.start();
	MutexLockGuard lock(mutex_);
	{
		while(loop_==nullptr)
		{
			cond_.wait();
		}
		return loop_;
	}
}

void EventLoopThread::threadFunc()
{
	{
	MutexLockGuard lock(mutex_);
	loop_ = new EventLoop();
	cond_.notify();
	}
	loop_->loop();
}



};
