#include "EventLoop.h"
#include "Channel.h"
#include "TcpConnection.h"
#include "Logging.h"
#include <unistd.h>
#include <assert.h>
#include <string.h>

namespace JINFENG{

TcpConnection::TcpConnection(EventLoop* loop, std::string name, int connfd, IPv4Address& localAddr, IPv4Address& peerAddr)
	:loop_(loop),
	socket_(new Socket(connfd)),
	localAddr_(localAddr),
	peerAddr_(peerAddr),
	channel_(new Channel(loop, connfd)),
	state_(kConnecting),
	name_(name)
{
	channel_->enableReading(true);
	channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
	channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
	channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
}

TcpConnection::~TcpConnection()
{
	LOG_INFO<<"decons tcpConnection: "<<name_;
}

//ET模式下读数据需要一次读完直到返回-1，errno 为 EAGAIN
void TcpConnection::handleRead()
{
	int savedErrno;
	ssize_t n = 0, ret;
	while((ret=inputBuffer_.readFd(socket_->fd(), &savedErrno))!=-1)
		n += ret;
	if(savedErrno!=EAGAIN)
		assert(true);
	LOG_INFO<<"receieve "<<n<<" Bytes";
	if(n>0){
		messageCallback_(shared_from_this());
	}
	else if(n==0)
	{
		LOG_TRACE<<"peer close";
		handleClose();
	}
	else
	{
		LOG_ERROR<<"receive data error";
		handleError();
	}
}


void TcpConnection::handleWrite()
{
	if(channel_->writeEnabled())
	{
		if(outputBuffer_.readableBytes()<0)
			return;
		ssize_t n = ::write(channel_->fd(),
				outputBuffer_.beginRead(),
				outputBuffer_.readableBytes());
		if(n>0)
		{
			outputBuffer_.retrieve(n);
			if(outputBuffer_.readableBytes()>0)
			{
				channel_->enableWriting(true);
			}
		}
		else
		{
			LOG_ERROR<<"TcpConnection::handleWrite";
		}
	}
}


void TcpConnection::handleError()
{
	
}

void TcpConnection::handleClose()
{
	closeCallback_(shared_from_this());
}

void TcpConnection::connectEstablished()
{
	setState(kConnected);
	connectionCallback_(shared_from_this());
}


void TcpConnection::send(const std::string& message)
{
	ssize_t nwrote;
	if(outputBuffer_.readableBytes()==0)
	{
		nwrote = ::write(channel_->fd(), message.data(), message.size());
		if(nwrote>=0)
		{
			if(static_cast<size_t>(nwrote)<message.size())
			{
				LOG_TRACE<<"I am going to send more data";
			}
		}
		else
		{
			nwrote = 0;
			if(errno!=EWOULDBLOCK)
				LOG_ERROR<<"TcpConnection::send";
		}
		
	}

	assert(nwrote>=0);

	if(static_cast<size_t>(nwrote)<message.size())
	{
		outputBuffer_.append(message.data()+nwrote, message.size()-nwrote);
		if(!channel_->writeEnabled())
			channel_->enableWriting(true);
	}
}



};

