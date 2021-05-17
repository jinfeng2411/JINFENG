#include "EventLoop.h"
#include "Channel.h"
#include "TcpConnection.h"
#include "Logging.h"
#include <unistd.h>
#include <assert.h>
#include <string.h>

namespace JINFENG{

TcpConnection::TcpConnection(EventLoop* loop, int connfd, IPv4Address& localAddr, IPv4Address& peerAddr)
	:loop_(loop),
	socket_(new Socket(connfd)),
	localAddr_(localAddr),
	peerAddr_(peerAddr),
	channel_(new Channel(loop, connfd))
{
	channel_->enableReading();
	channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::handleRead()
{
	int savedErrno;
	inputBuffer_.readFd(socket_->fd(), &savedErrno);
	messageCallback_(shared_from_this());
}


};

