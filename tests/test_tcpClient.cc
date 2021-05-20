#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpClient.h"
#include "Logging.h"
#include <unistd.h>
#include <iostream>
#include "TcpConnection.h"

void messageCallback(const JINFENG::TcpConnection::ptr& ptr)
{
	std::cout<<ptr->inputBuffer().retrieveAllAsString()<<std::endl;
}

void test_connector()
{
	JINFENG::EventLoop *loop = new JINFENG::EventLoop();
	JINFENG::IPv4Address servaddr("127.0.0.1", 9999);
	JINFENG::TcpClient client(loop, servaddr);
	client.setMessageCallback(messageCallback);
	client.connect();

	loop->loop();
}


int main()
{
	test_connector();
}
