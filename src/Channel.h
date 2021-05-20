#pragma once

#include <functional>
#include <sys/epoll.h>

namespace JINFENG{

class EventLoop;
class Channel{
public:
	typedef std::shared_ptr<Channel> ptr;
	typedef std::function<void()> ReadCallback;
	typedef std::function<void()> WriteCallback;
	typedef std::function<void()> CloseCallback;
	Channel(EventLoop* loop, int fd);
	~Channel();

	int fd() {return fd_;}

	//注册监听事件
	void enableReading(bool on)
	{
		if(on)
			events_ |= EPOLLIN;
		else
			events_ &= (~EPOLLIN);
		loop_->updateChannel(this);
	}

	void enableWriting(bool on)
	{
		if(on)
			events_ |= EPOLLOUT;
		else
			events_ &= (~EPOLLOUT);
		loop_->updateChannel(this);
	}

	bool readEnabled()
	{
		return events_ & EPOLLIN;
	}

	bool writeEnabled()
	{
		return events_ & EPOLLOUT;
	}

	uint32_t events() {return events_;}
	uint32_t revents() {return revents_;}
	void setRevents(uint32_t revents) {revents_ = revents;}

	//注册事件触发时的回调
	void setReadCallback(ReadCallback readCallback)
	{
		readCallback_ = readCallback;
	}
	void setWriteCallback(WriteCallback writeCallback)
	{
		writeCallback_ = writeCallback;
	}
	void setCloseCallback(CloseCallback closeCallback)
	{
		closeCallback_ = closeCallback;
	}

	void handleEvent();
private:
	EventLoop* loop_;
	int fd_;  //监听的fd
	uint32_t events_;  //监听的事件
	uint32_t revents_; //触发的事件
	ReadCallback readCallback_; //写事件回调
	WriteCallback writeCallback_; //读事件回调
	CloseCallback closeCallback_; //对端关闭事件回调
};

}
