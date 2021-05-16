#pragma once

#include <map>
#include <memory>
#include <sys/timerfd.h>
#include <unistd.h>
#include <assert.h>
#include <Timer.h>
#include <string.h>

namespace JINFENG{

class EventLoop;
class Channel;
//定时器精度，微秒级别
class TimerQueue{
public:
	typedef Timer::Callback Callback;
	TimerQueue(EventLoop* loop);
	~TimerQueue();

	void addTimer(Timer*);
private:
	//定时时间到触发读回调
	void handleRead();

	//更新 timerfd 的触发事件
	void refreshEarliest();
private:
	EventLoop* loop_;
	int timerfd_;
	std::unique_ptr<Channel> channel_;
	std::map<uint64_t, Timer*> timers_; //触发时间点，相应的定时器
};	


};
