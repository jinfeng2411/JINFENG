#include "TcpServer.h"
#include "EventLoop.h"
#include "Channel.h"
#include "TcpConnection.h"
#include "Acceptor.h"
#include "Logging.h"

namespace JINFENG{

TcpServer::TcpServer(EventLoop* loop, const IPv4Address& addr, std::string name)
	:loop_(loop),
	servAddr_(addr),
	acceptor_(new Acceptor(loop_, servAddr_)),
	name_(name),
	count_(1)
{
	
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
	acceptor_->listen();
	acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::newConnection(int sockfd, const IPv4Address& peerAddr)
{
	LOG_TRACE<<"accepted a new connection from "<<peerAddr.ip()<<":"<<peerAddr.port()<<", connfd="<<sockfd;
	std::string connName = name_ + std::to_string(count_);
	++count_;
	IPv4Address addr = peerAddr; //fix me
	TcpConnectionPtr ptr(new TcpConnection(loop_, sockfd, servAddr_, addr));
	ptr->setMessageCallback(messageCallback_);
	connections_[connName] = ptr;
}




};
