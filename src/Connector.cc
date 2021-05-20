#include "EventLoop.h"
#include "Channel.h"
#include "Connector.h"
#include "TcpConnection.h"
#include "Logging.h"

namespace JINFENG{

Connector::Connector(EventLoop* loop, const IPv4Address& servaddr):
	loop_(loop),
	servaddr_(servaddr),
	socket_(new Socket()),
	channel_(new Channel(loop_, socket_->fd()))
{

	LOG_TRACE<<"Connector::Connector";
}

Connector::~Connector()
{

}

void Connector::start()
{
	socket_->connect(servaddr_);
	Socket::setNonBlock(socket_->fd(), true);
	channel_->setWriteCallback(std::bind(&Connector::handleWrite,this));
	//channel_->setErrorCallback(std::bind(&Connector::handleError,this));
	channel_->enableWriting(true);
}

void Connector::restart()
{

}


void Connector::stop()
{
}

void Connector::handleWrite()
{
	LOG_TRACE<<"Connector::handleWrite";
	//loop_->removeChannel(channel_);	
	delete channel_;
	newConnectionCallback_(socket_->fd());
}

void Connector::handleError()
{
}


};
