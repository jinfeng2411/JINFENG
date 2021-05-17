#pragma once

#include "InetAddress.h"
#include "Socket.h"
#include <functional>
#include <memory>

namespace JINFENG{

class EventLoop;
class Channel;

class Acceptor{
public:
	typedef std::function<void(int sockfd, const IPv4Address&)> NewConnectionCallback;

	Acceptor(EventLoop* loop, const IPv4Address& listenAddr, bool reuseport=true);
	~Acceptor();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{
		newConnectionCallback_ = cb;
	}

	void listen();
private:
	void handleRead();

private:
	EventLoop* loop_;
	std::unique_ptr<Socket> acceptSocket_;
	std::unique_ptr<Channel> acceptChannel_;
	NewConnectionCallback newConnectionCallback_;
};	



};
