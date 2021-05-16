#include "EventLoop.h"
#include "Channel.h"
#include "TimerQueue.h"
#include "Logging.h"
#include <errno.h>
#include <string.h>

namespace JINFENG{

TimerQueue::TimerQueue(EventLoop* loop):
	loop_(loop),
	timerfd_(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC)),
	channel_(new Channel(loop_, timerfd_))
{
	assert(timerfd_!=-1);
	channel_->setReadCallback(std::bind(&TimerQueue::handleRead, this));
	channel_->enableReading();
}

TimerQueue::~TimerQueue(){

}

void TimerQueue::addTimer(Timer* timer)
{
	uint64_t micro = timer->expiration();
	uint64_t left = micro-Timer::nowTimeMicro();
	if(left<0)
	{
		LOG_WARN<<"the timer's expired time early than now time";
		return;
	}
	refreshEarliest();
	timers_[timer->expiration()] = timer;
	LOG_TRACE<<"added a Timer which will occur after "<<left/(1000*1000)<<"."<<left%(1000*1000)<<" seconds";
}

void TimerQueue::handleRead()
{
	LOG_TRACE<<"some Timers expired";
	char msg[128];
	int ret = ::read(timerfd_, msg, sizeof(msg));
	assert(ret>=0);

        //获取过期的定时器
        uint64_t now = Timer::nowTimeMicro();
	int count = 1;
        while(!timers_.empty() && timers_.begin()->first<=now)
        {	
		LOG_TRACE<<"handling "<<count<<" Timer";
		Timer* t = timers_.begin()->second;
                t->cb()();
                timers_.erase(timers_.begin());
		LOG_TRACE<<"finished handle expired Timers";

                //循环定时器，则重新更新定时时间
                if(t->isRepeat())
                {
			uint64_t micro = t->expiration()+t->interval();
			Timer* newT = new Timer(micro, t->cb(), t->interval());
                        addTimer(newT);
                }
		
                //删除旧的定时器
                delete t;
        }
	refreshEarliest();
}

void TimerQueue::refreshEarliest()
{
	//TimerQueue为非空更新
        if(!timers_.empty())
	{
		uint64_t micro = timers_.begin()->first;
		struct itimerspec newVal, oldVal;
		bzero(&newVal, sizeof(newVal));
		bzero(&oldVal, sizeof(oldVal));
		newVal.it_value.tv_sec = (micro-Timer::nowTimeMicro())/(1000*1000);
		newVal.it_value.tv_nsec = ((micro-Timer::nowTimeMicro())%(1000*1000))*1000;
                int ret = ::timerfd_settime(timerfd_, 0, &newVal, &oldVal);
                if(ret)
			LOG_ERROR<<strerror(errno);
		assert(ret==0);
        }
}



};
