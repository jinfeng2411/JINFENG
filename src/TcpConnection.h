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
	enum StateE {
                kConnecting,
                kConnected
        };

	typedef std::shared_ptr<TcpConnection> ptr;
	typedef std::function<void(const TcpConnection::ptr&)> ConnectionCallback;
	typedef std::function<void(const TcpConnection::ptr&)> MessageCallback;
	typedef std::function<void(const TcpConnection::ptr&)> CloseCallback;

	TcpConnection(EventLoop* loop, std::string name, int connfd, IPv4Address& localAddr, IPv4Address& peerAddr);
	~TcpConnection();

	std::string name() {return name_;}

	void setMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}

	void setCloseCallback(const CloseCallback& cb)
	{
		closeCallback_ = cb;
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

	void send(const std::string& message);


	void connectEstablished();
private:
	//读入 inputBuffer_, 再调用相应的回调
	void handleRead();

	//
	void handleClose();

	//
	void handleError();

	//写入 outputBuffer_
	void handleWrite();


	void setState(StateE state)
	{
		state_ = state;
	}	

private:
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
	CloseCallback closeCallback_;
};



};
