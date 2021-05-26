#pragma once

#include <memory>
#include "Socket.h"
#include "InetAddress.h"
#include "Connector.h"
#include "TcpConnection.h"
#include <assert.h>

namespace JINFENG{

class EventLoop;

class TcpClient{
public:
	typedef TcpConnection::MessageCallback MessageCallback;
	typedef TcpConnection::ConnectionCallback ConnectionCallback;
	TcpClient(EventLoop* loop, const IPv4Address& servaddr);
	~TcpClient();

	void setMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}

	void send(const std::string& msg)
	{
		assert(state_==kConnected);
		connection_->send(msg);
	}
	
	void connect();

	int fd()
	{
		return connector_->fd();
	}
private:
	void newConnection(int fd);
private:
	enum stateE{
		kConnecting,
		kConnected,
		kDisconnected,
	};
	EventLoop* loop_;
	IPv4Address servaddr_;
	stateE state_;
	std::unique_ptr<Connector> connector_;
	std::shared_ptr<TcpConnection> connection_;
	MessageCallback messageCallback_;
	ConnectionCallback connectionCallback_;
};



};
