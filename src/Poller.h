#pragma once

#include <sys/epoll.h>
#include <unordered_set>

namespace JINFENG{

class EventLoop;
//采用 epoll 的 ET 模式
class EPoller{
public:
	typedef Channel::ptr ChannelPtr;

	EPoller(EventLoop* loop);
	~EPoller();

	void addChannel(Channel* channel);
	void removeChannel(Channel* channel);
	void updateChannel(Channel* channel);

	void loop_once();
	
private:
	const static int kMaxEvents = 1024;
private:
	EventLoop* loop_;
	int epfd_;
	std::unordered_set<Channel*> channels_;
	int lastActive_;
	struct epoll_event activeEvents_[kMaxEvents];
};	


};
