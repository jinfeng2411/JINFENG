#include "EventLoop.h"
#include "TcpConnection.h"
#include "HttpServer.h"

int main()
{
	JINFENG::EventLoop loop;
	JINFENG::IPv4Address listenAddr("127.0.0.1", 9999);
	JINFENG::HttpServer server(&loop, listenAddr);
	server.start();
	loop.loop();
}
