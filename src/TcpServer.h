#pragma once

#include <memory>
#include <functional>
#include <string>
#include <map>
#include "InetAddress.h"
#include "Socket.h"
#include "TcpConnection.h"

namespace JINFENG{

class EventLoop;
class Channel;
class TcpConnection;
class Acceptor;
class EventLoopThreadPool;

class TcpServer{
public:
	typedef TcpConnection::ptr TcpConnectionPtr;
	typedef TcpConnection::MessageCallback MessageCallback;
	typedef TcpConnection::ConnectionCallback ConnectionCallback;
	TcpServer(EventLoop* loop, const IPv4Address& addr, int numIOThreads=0, std::string name="jinfeng-tcpServer");
	~TcpServer();

	void start();

	void setMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}
	
private:
	void newConnection(int sockfd, const IPv4Address& peerAddr);

	void removeConnection(const TcpConnectionPtr&);
private:
	typedef std::map<std::string, TcpConnectionPtr> ConnMap;
	EventLoop* loop_;
	std::string name_;
	IPv4Address servAddr_;
	std::unique_ptr<Acceptor> acceptor_;
	MessageCallback messageCallback_;
	ConnectionCallback connectionCallback_;
	ConnMap connections_;
	int count_;
	std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
};


};
