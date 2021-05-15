#include "ThreadPool.h"
#include "Logging.h"

namespace JINFENG{

ThreadPool::ThreadPool()
	:running_(false),
	numThreads_(0),
	mutex_(),
	cond_(mutex_)
{

}

ThreadPool::~ThreadPool()
{
	if(running_)
		stop();
}

void ThreadPool::start(int numThreads)
{
	LOG_TRACE<<"ThreadPool::start";
	assert(!running_);
	assert(threads_.empty());
	numThreads_ = numThreads;
	threads_.reserve(numThreads_);
	running_ = true;
	for(int i=0; i<numThreads; ++i)
	{
		threads_.push_back(new Thread(std::bind(&ThreadPool::run, this)));
		threads_[i]->start();
		LOG_TRACE<<"create thread";
	}
}

void ThreadPool::stop()
{
	assert(running_);
	running_ = false;
	cond_.notifyAll();
	for(auto& thread: threads_){
		thread->join();
		delete thread;
		LOG_TRACE<<"delete thread";
	}
	threads_.clear();
}

void ThreadPool::run()
{
	while(running_)
	{
		Task t = takeTask();
		if(t)
			t();
	}
}

void ThreadPool::addTask(const Task& task)
{
	MutexLockGuard lock(mutex_);
	tasks_.push_back(task);
	LOG_TRACE<<"ThreadPool::addTask";
	cond_.notify();
}

ThreadPool::Task ThreadPool::takeTask()
{
	MutexLockGuard lock(mutex_);
	//running_ 的判断用于 stop
	while(tasks_.empty() && running_)
	{
		cond_.wait();
	}
	Task t;
	//可能是由 stop 命令触发
	if(!tasks_.empty())
	{
		t = tasks_.front();
		tasks_.pop_front();
	}
	return t;
}


};
