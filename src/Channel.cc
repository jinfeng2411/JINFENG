#include "EventLoop.h"
#include "Channel.h"
#include "Logging.h"
#include <unistd.h>

namespace JINFENG{

Channel::Channel(EventLoop* loop, int fd)
	:loop_(loop), 
	fd_(fd),
	events_(0),
	revents_(0)
{

}

Channel::~Channel()
{
	if(fd_>=0)
	{
		LOG_TRACE<<"remove Channel, fd="<<fd_;
		loop_->removeChannel(this);
	}
}

void Channel::handleEvent()
{

	LOG_TRACE<<"Channel::handleEvent";
	if(revents_ & EPOLLRDHUP)
	{
		LOG_TRACE<<"EPOLLRDHUP(peer close the connection) on fd:"<<fd_;
		if(closeCallback_){
			closeCallback_();
		}
	}
	else if(revents_ & EPOLLIN)
	{
		LOG_TRACE<<"detected read event on fd:"<<fd_;
		if(readCallback_)
			readCallback_();
	}
	if(revents_ & EPOLLOUT)
	{
		LOG_TRACE<<"detected write event on fd:"<<fd_;
		if(writeCallback_)
			writeCallback_();
	}	
}


};
