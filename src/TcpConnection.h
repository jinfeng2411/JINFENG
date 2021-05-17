#pragma once

#include <memory>
#include <string>
#include "InetAddress.h"
#include "Socket.h"
#include "Buffer.h"

namespace JINFENG{

class EventLoop;
class Channel;

class TcpConnection: public std::enable_shared_from_this<TcpConnection>{
public:
	typedef std::shared_ptr<TcpConnection> ptr;
	typedef std::function<void(const TcpConnection::ptr&)> ConnectionCallback;
	typedef std::function<void(const TcpConnection::ptr&)> MessageCallback;

	TcpConnection(EventLoop* loop, int connfd, IPv4Address& localAddr, IPv4Address& peerAddr);
	~TcpConnection();

	void setMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

	Buffer& inputBuffer()
	{
		return inputBuffer_;
	}

	Buffer& outputBuffer()
	{
		return outputBuffer_;
	}

	IPv4Address peerAddr()
	{
		return peerAddr_;
	}
private:
	void handleRead();

private:
	enum StateE {
		kConnecting,
		kConnected
	};

	std::string name_;
	StateE state_;

	EventLoop* loop_;
	std::unique_ptr<Socket> socket_;
	IPv4Address localAddr_;
	IPv4Address peerAddr_;
	std::unique_ptr<Channel> channel_;
	Buffer inputBuffer_;
	Buffer outputBuffer_;

	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
};



};
