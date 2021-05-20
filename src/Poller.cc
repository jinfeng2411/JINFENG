#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include "Logging.h"
#include <string.h>
#include <assert.h>

namespace JINFENG{

EPoller::EPoller(EventLoop* loop):
	loop_(loop),
	epfd_(epoll_create(1))
{
}

EPoller::~EPoller()
{

}

//新增监听事件
void EPoller::addChannel(Channel* channel)
{
	assert(channels_.find(channel)==channels_.end());
	epoll_event event;
	bzero(&event, sizeof(event));
	event.events = channel->events();
	event.events |= EPOLLET;
	event.events |= EPOLLRDHUP;
	event.events |= EPOLLHUP;
	event.data.fd = channel->fd();
	event.data.ptr = channel;
	int ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, channel->fd(), &event);
	assert(ret==0);
	LOG_TRACE<<"added Channel fd="<<channel->fd()<<" read?"<<(channel->events()&EPOLLIN)<<" write?"<<(channel->events()&EPOLLOUT);
	channels_.insert(channel);
}

void EPoller::removeChannel(Channel* channel)
{
	assert(channels_.find(channel)!=channels_.end());
	channels_.erase(channel);
	int ret = epoll_ctl(epfd_, EPOLL_CTL_DEL, channel->fd(), NULL);
	assert(ret==0);
	LOG_TRACE<<"deleted Channel fd="<<channel->fd();
}

void EPoller::updateChannel(Channel* channel)
{
	if(channels_.find(channel)==channels_.end()) //新增加的 Channel
	{
		addChannel(channel);
	}
	else{ //非新 Channel
		struct epoll_event event;
		bzero(&event, sizeof(event));
		event.events = channel->events();
		event.events |= EPOLLET;
		event.events |= EPOLLRDHUP;
		event.events |= EPOLLHUP;
		event.data.fd = channel->fd();
		event.data.ptr = channel;
		int ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, channel->fd(), &event);
		assert(ret==0);
		LOG_TRACE<<"added Channel fd="<<channel->fd()<<" read?"<<(channel->events()&EPOLLIN)<<" write?"<<(channel->events()&EPOLLOUT);
	}
}

void EPoller::loop_once()
{
	LOG_TRACE<<"begin EPoller::loop_once";
	lastActive_ = epoll_wait(epfd_, activeEvents_, kMaxEvents, -1);
	LOG_TRACE<<lastActive_<<" Channels ready for request I/O";
	assert(lastActive_>=0);
	for(int i=0; i<lastActive_; ++i)
	{
		Channel* channel = (Channel*)activeEvents_[i].data.ptr;
		int events = activeEvents_[i].events;
		channel->setRevents(events);
		channel->handleEvent();
	}	
}

};
