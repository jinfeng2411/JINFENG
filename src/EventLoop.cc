#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include "TimerQueue.h"
#include "Logging.h"

namespace JINFENG{

EventLoop::EventLoop()
	:threadId_(getTid()),
	poller_(new EPoller(this)),
	wakeupFd_(eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK)),
	wakeupChannel_(new Channel(this, wakeupFd_)),
	timers_(new TimerQueue(this))
{

}

EventLoop::~EventLoop()
{

}

void EventLoop::addChannel(Channel* channel)
{
	poller_->addChannel(channel);
}

void EventLoop::updateChannel(Channel* channel)
{
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
	poller_->removeChannel(channel);
}

void EventLoop::runAt(uint64_t us, Task task)
{
	Timer* t = new Timer(us, task);
	timers_->addTimer(t);
}

void EventLoop::runAfter(double delayS, Task task)
{
	uint64_t at = Timer::nowTimeMicro() + delayS*1000*1000;
	runAt(at, task);
}

void EventLoop::runEvery(double intervalS, Task task)
{
	uint64_t at = Timer::nowTimeMicro() + intervalS*1000*1000;
	uint64_t interval = intervalS*1000*1000;
	Timer* t = new Timer(at, task, interval);
	timers_->addTimer(t);
}



void EventLoop::loop()
{
	LOG_TRACE<<"begin EventLoop::loop";
	while(1)
	{
		poller_->loop_once();
	}
}


};
