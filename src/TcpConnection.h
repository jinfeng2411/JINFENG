#pragma once

#include <memory>
#include <string>
#include "InetAddress.h"
#include "Socket.h"
#include "Buffer.h"
#include "JINFENG-impl.h"

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

	template<class T>
	T& context()
	{
		return ctx_.context<T>();
	}
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
	IPv4Address localAddr_;  //本端地址
	IPv4Address peerAddr_; //对端地址
	std::unique_ptr<Channel> channel_; //该连接的Channel
	Buffer inputBuffer_;  //输入缓冲
	Buffer outputBuffer_;  //输出缓冲

	AutoContext ctx_;  //将来可以扩展

	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	CloseCallback closeCallback_;
};



};
