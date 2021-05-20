#include "TcpServer.h"
#include "EventLoop.h"
#include "Channel.h"
#include "TcpConnection.h"
#include "Acceptor.h"
#include "Logging.h"
#include "EventLoopThreadPool.h"

namespace JINFENG{

TcpServer::TcpServer(EventLoop* loop, const IPv4Address& addr, int numIOThreads, std::string name)
	:loop_(loop),
	servAddr_(addr),
	acceptor_(new Acceptor(loop_, servAddr_)),
	name_(name),
	count_(1),
	eventLoopThreadPool_(new EventLoopThreadPool(loop_))
{
	eventLoopThreadPool_->start(numIOThreads);
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
	std::string connName = name_ + "_" + std::to_string(count_);
	++count_;
	IPv4Address addr = peerAddr; //fix me
	EventLoop* loop = eventLoopThreadPool_->getNextLoop();
	TcpConnectionPtr ptr(new TcpConnection(loop, connName, sockfd, servAddr_, addr));
	ptr->setConnectionCallback(connectionCallback_);
	ptr->setMessageCallback(messageCallback_);
	ptr->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
	connections_[connName] = ptr;
	ptr->connectEstablished();
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO<<"TcpServer::removeConnection ["<<conn->name()<<"]";
	connections_.erase(conn->name());
}


};
