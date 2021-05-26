#include "TcpClient.h"
#include "EventLoop.h"
#include "Logging.h"

namespace JINFENG{

TcpClient::TcpClient(EventLoop* loop, const IPv4Address& servaddr)
	:loop_(loop),
	servaddr_(servaddr),
	state_(kDisconnected),
	connector_(new Connector(loop, servaddr))
{

}

TcpClient::~TcpClient()
{

}


void TcpClient::connect()
{
	state_ = kConnecting;
	connector_->setNewConnectionCallback(std::bind(&TcpClient::newConnection, this, std::placeholders::_1));
	connector_->start();
	state_ = kConnected;
}


void TcpClient::newConnection(int fd)
{
	IPv4Address localAddr;  //fix me
	connection_.reset(
			new TcpConnection(loop_, 
				"jinfeng-client",
			        connector_->fd(),	
				localAddr, 
				servaddr_)	
	);
	connection_->setMessageCallback(messageCallback_);
	connectionCallback_(connection_);
}


};
