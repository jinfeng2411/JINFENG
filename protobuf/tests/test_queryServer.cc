#include "ProtobufCodec.h"
#include "ProtobufDispatcher.h"
#include "Logging.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "Query.pb.h"
#include <functional>


using namespace JINFENG;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;


class QueryServer{
public:
	typedef TcpConnection::ptr TcpConnectionPtr;
	QueryServer(EventLoop* loop, const IPv4Address& listenAddr)
		:server_(loop, listenAddr),
		dispatcher_(),
		codec_(std::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2))
	{
	
		dispatcher_.registerMessageCallback<Query>(
				std::bind(&QueryServer::onQuery, this, _1, _2));
		dispatcher_.registerMessageCallback<Answer>(
				std::bind(&QueryServer::onAnswer, this, _1, _2));
		server_.setConnectionCallback(
				std::bind(&QueryServer::onConnection, this, _1));
		server_.setMessageCallback(
				std::bind(&ProtobufCodec::onMessage, &codec_, _1));
				
	}

	void start()
	{
		server_.start();
	}

private:
	void onConnection(const TcpConnectionPtr& conn)
	{

	}


	void onQuery(const TcpConnectionPtr& conn, Query* query)
	{
		LOG_TRACE<<"onQuery";
		LOG_TRACE<<query->id();
		LOG_TRACE<<query->questioner();
		for(int i=0; i<query->question_size(); ++i)
		{
			LOG_TRACE<<query->question(i);
		}
	}


	void onAnswer(const TcpConnectionPtr& conn, Answer* answer)
	{
		LOG_TRACE<<"onAnswer";
		LOG_TRACE<<answer->id();
		LOG_TRACE<<answer->questioner();
		LOG_TRACE<<answer->answer();
		for(int i=0; i<answer->solution_size(); ++i)
		{
			LOG_TRACE<<answer->solution(i);
		}
	}
private:
	TcpServer server_;
	ProtobufDispatcher dispatcher_;
	ProtobufCodec codec_;
};

int main()
{
	EventLoop loop;
	IPv4Address listenAddr("127.0.0.1", 9999);
	QueryServer server(&loop, listenAddr);
	server.start();
	loop.loop();
}

/*
int main()
{
	ProtobufDispatcher dispatcher;
}
*/
