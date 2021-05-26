#include "ProtobufCodec.h"
#include "ProtobufDispatcher.h"
#include "Logging.h"
#include "EventLoop.h"
#include "Buffer.h"
#include "TcpClient.h"
#include "TcpConnection.h"
#include "Query.pb.h"
#include <unistd.h>

using namespace JINFENG;

void onMsg(const TcpConnection::ptr& conn)
{

}

void onConnection(const TcpConnection::ptr& conn)
{
        Buffer buf;
        Query query;
        query.set_id(11);
        query.set_questioner("jin");
        query.add_question("how");
	query.add_question("what");

	ProtobufCodec::encode(query, buf);
	//测试分两次发送一条消息
	conn->send(buf.retrieveAsString(buf.readableBytes()-1));
	sleep(3);
	conn->send(buf.retrieveAllAsString());


	sleep(1);

	buf.retrieveAll();
	Answer answer;
	answer.set_id(11);
	answer.set_questioner("jin");
	answer.set_answer("feng");
	answer.add_solution("this");
	answer.add_solution("that");
	ProtobufCodec::encode(answer, buf);
	conn->send(buf.retrieveAllAsString());
}


class QueryClient{
public:	
	QueryClient(EventLoop* loop, const IPv4Address& servaddr)
		:client_(loop, servaddr)
	{
		client_.setMessageCallback(onMsg);
		client_.setConnectionCallback(onConnection);	
	}

	void connect()
	{
		client_.connect();
	//	client_.send(buf->retrieveAllAsString());
	}

private:
	TcpClient client_;
};


int main()
{
	EventLoop loop;
	IPv4Address servaddr("127.0.0.1", 9999);

        QueryClient client(&loop, servaddr);
	client.connect();
	
	loop.loop();
}	
