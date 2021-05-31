#pragma once

#include <functional>
#include "Logging.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace JINFENG{

class HttpServer{
public:
	typedef TcpConnection::ptr TcpConnectionPtr;
	typedef std::function<void(const HttpRequest&, HttpResponse&)>  HttpCallback;

	HttpServer(EventLoop* loop, const IPv4Address& listenAddr, int numIOThreads=0, std::string name="jinfeng-httpServer"): server_(loop, listenAddr, numIOThreads, name)
	{
		server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
		server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1));
	}
	
	void start()
	{
		LOG_TRACE<<"HttpServer["<<server_.name()<<"] start listenning on "<<server_.ip()<<":"<<server_.port();
		server_.start();
	}

	void setHttpCallback(const std::string& method, const std::string& uri, const HttpCallback& cb)
	{
		cbs_[method][uri] = cb;
	}

	HttpRequest& getRequest(const TcpConnectionPtr& conn)
	{
		return conn->context<HttpContext>().req;
	}


	HttpResponse& getResponse(const TcpConnectionPtr& conn)
	{
		return conn->context<HttpContext>().resp;
	}


private:
	void onConnection(const TcpConnectionPtr&);

	//在 onMessage 中尝试 decode TcpConnection 中的 Buffer
	//若解析出一个完整的 HttpRequest；
	//则调用 httpCallback_ 构造 HttpResponse
	void onMessage(const TcpConnectionPtr&);

	void defaultHttpCallback(const HttpRequest& req, HttpResponse& resp);	

private:
	struct HttpContext{
		HttpRequest req;
		HttpResponse resp;
	};

	TcpServer server_;
	std::map<std::string, std::map<std::string, HttpCallback>> cbs_;
};



};
