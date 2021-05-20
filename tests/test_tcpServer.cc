#include "InetAddress.h"
#include "Socket.h"
#include "Logging.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include <iostream>
using namespace std;

void testInetAddr()
{
        JINFENG::IPv4Address addr("127.0.0.1", 9999);
        cout<<addr.ip()<<endl;
        cout<<addr.port()<<endl;
}

void testSocket()
{
	JINFENG::Socket socket;
	JINFENG::IPv4Address address("127.0.0.1", 9999);
	socket.bind(address);
	socket.listen();
	while(1)
	{
		JINFENG::IPv4Address peerAddr;
		int connfd = socket.accept(peerAddr);
		LOG_TRACE<<"accepted connection from "<<peerAddr.ip()<<" : "<<peerAddr.port()<<", connfd="<<connfd;
	}
}

void connectionCallback(const JINFENG::TcpConnection::ptr& ptr)
{
	ptr->send("hello from server");
}

void messageCallback(const JINFENG::TcpConnection::ptr& ptr)
{
	std::string msg = ptr->inputBuffer().retrieveAllAsString();
	LOG_TRACE<<"receive message from "<<ptr->peerAddr().ip()<<":"<<ptr->peerAddr().port()<<" "<<msg;
//	msg = msg + "server reply";
//	ptr->send(msg);
}


void testTcpServer()
{
	JINFENG::EventLoop* loop = new JINFENG::EventLoop();
	JINFENG::IPv4Address servaddr("127.0.0.1", 9999);
	JINFENG::TcpServer server(loop, servaddr, 3);
	server.setMessageCallback(messageCallback);
	server.setConnectionCallback(connectionCallback);
	server.start();
	loop->loop();
}

int main()
{
	//testInetAddr();
	//testSocket();
	testTcpServer();
}
