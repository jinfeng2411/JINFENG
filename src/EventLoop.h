#pragma once

#include <memory>
#include <vector>
#include <sys/eventfd.h>
#include "utils.h"
#include <functional>

namespace JINFENG{

class Channel;
class TimerQueue;
class EPoller;
class EventLoop{
public:
	typedef std::function<void()> Task;
	EventLoop();
	~EventLoop();


	void addChannel(Channel* channel);
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);

	//定时器调用接口
	void runAt(uint64_t us, Task task);
	void runAfter(double delayS, Task task);
	void runEvery(double intervalS, Task task);
	
	void loop();

private:
	//用来处理 wakeupChannel_ 的读事件
	void handleRead();

	pid_t threadId_;
	std::unique_ptr<EPoller> poller_;
	int wakeupFd_;  //唤醒(eventfd)
	std::unique_ptr<Channel> wakeupChannel_;
	std::unique_ptr<TimerQueue> timers_;  //定时器(timerfd)
};


};
