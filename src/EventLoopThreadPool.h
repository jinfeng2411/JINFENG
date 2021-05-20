#pragma once

#include "EventLoopThread.h"
#include <vector>

namespace JINFENG{

class EventLoopThreadPool{
public:
	EventLoopThreadPool(EventLoop* baseLoop);
	~EventLoopThreadPool();

	void start(int numThreads);
	EventLoop* getNextLoop();

private:
	EventLoop* baseLoop_;  //在 tcpServer 中即为接受新连接的 EventLoop
	bool start_;
	int numThreads_;
	int next_;
	std::vector<EventLoopThread*> threads_;
	std::vector<EventLoop*> loops_;
};


};
