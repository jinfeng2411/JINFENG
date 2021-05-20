#pragma once

#include <memory>
#include <functional>
#include "InetAddress.h"
#include "Socket.h"

namespace JINFENG{

class EventLoop;
class Channel;
class TcpConnection;

class Connector{
public:
	typedef std::function<void(int fd)> NewConnectionCallback;
	typedef std::function<void(int fd)> ErrorCallback;

	Connector(EventLoop* loop, const IPv4Address& servaddr);
	~Connector();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{
		newConnectionCallback_ = cb;
	}

	void setErrorCallback(const ErrorCallback& errorCallback)
	{
		errorCallback_ = errorCallback;
	}


	void start();

	void restart();

	void stop();

	int fd()
	{
		return socket_->fd();
	}

private:
	void handleWrite();
	void handleError();

	EventLoop* loop_;
	IPv4Address servaddr_;
	std::unique_ptr<Socket> socket_;
	Channel* channel_;
	NewConnectionCallback newConnectionCallback_;
	ErrorCallback errorCallback_;
};


};
