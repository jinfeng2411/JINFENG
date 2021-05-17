#include "Acceptor.h"
#include "EventLoop.h"
#include "Channel.h"

namespace JINFENG{

Acceptor::Acceptor(EventLoop* loop, const IPv4Address& listenAddr, bool reuseport)
	:loop_(loop),
	acceptSocket_(new Socket()),
	acceptChannel_(new Channel(loop_, acceptSocket_->fd()))
{
	acceptSocket_->bind(listenAddr);
	Socket::setNonBlock(acceptSocket_->fd(), true);
	Socket::setReuseAddr(acceptSocket_->fd(), true);
	Socket::setReusePort(acceptSocket_->fd(), true);
}

Acceptor::~Acceptor()
{

}

void Acceptor::listen()
{
	acceptSocket_->listen();
	acceptChannel_->setReadCallback(std::bind(&Acceptor::handleRead, this));
	acceptChannel_->enableReading();
}

void Acceptor::handleRead()
{
	IPv4Address peerAddr;
	int connfd = acceptSocket_->accept(peerAddr);
	if(newConnectionCallback_)
		newConnectionCallback_(connfd, peerAddr);
}

};
