#include "EventLoopThreadPool.h"
#include "Logging.h" 

namespace JINFENG{


EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
	:baseLoop_(baseLoop),
	start_(false),
	numThreads_(0),
	next_(0)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{
	for(int i=0; i<numThreads_; ++i)
	{
		delete threads_[i];
		delete loops_[i];
		LOG_INFO<<"Destroying "<<i+1<<" EventLoopThread";
	}
}

void EventLoopThreadPool::start(int numThreads)
{
	assert(numThreads_==0);
	LOG_INFO<<"EventLoopThreadPool will create "<<numThreads<<" EventLoopThreads";
	numThreads_ = numThreads;
	threads_.reserve(numThreads);
	loops_.reserve(numThreads);
	for(int i=0; i<numThreads; ++i)
	{
		LOG_INFO<<"Creating "<<i+1<<" EventLoopThread";
		threads_[i] = new EventLoopThread();
		loops_[i] = threads_[i]->startLoop();
	}
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
	if(numThreads_==0)
		return baseLoop_;
	EventLoop* loop = loops_[next_];
	next_ = (next_+1)%numThreads_;
	return loop;
}




};
