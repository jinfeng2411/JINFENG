#include "HttpServer.h"
#include "Buffer.h"
#include "Logging.h"
#include "file.h"
#include "assert.h"

namespace JINFENG{

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
	LOG_TRACE<<"HttpServer::onConnection";
}

void HttpServer::onMessage(const TcpConnectionPtr& conn)
{
	//LOG_TRACE<<"HttpServer::onMessage";
	Buffer& buf = conn->inputBuffer();
	HttpRequest& req = getRequest(conn);
	if(req.tryDecode(buf)>0)
	{	
		LOG_TRACE<<"decode http response sucessfully";
		LOG_TRACE<<"Method:"<<req.method()<<" Uri:"<<req.uri();
//		HttpResponse& resp = getResponse(conn);
		HttpResponse resp;

		//寻找是否已注册对应 request 的回调
		//否则使用默认回调 404
		bool find = false;
		HttpCallback cb = std::bind(&HttpServer::defaultHttpCallback, this, std::placeholders::_1, std::placeholders::_2);
		if(cbs_.find(req.method())!=cbs_.end())
		{
			if(cbs_[req.method()].find(req.uri())!=cbs_[req.method()].end())
			{
				cb = cbs_[req.method()][req.uri()];
				find = true;
			}
		}
		if(!find)
			LOG_WARN<<"No Coresspoding Callback was found, using defalutCallback";
		cb(req, resp);
		Buffer output;
		resp.encode(output);
		std::string str = output.retrieveAllAsString();
		conn->send(str);
		//LOG_TRACE<<"\n"<<str;
	}
}


void HttpServer::defaultHttpCallback(const HttpRequest& req, HttpResponse& resp)
{
	resp.setVersion("HTTP/1.1");
	resp.setStatusCode(404);
	resp.setStatusWord("NOT_FOUND");

	std::string body;
	file::getContent("/home/jinfeng/linuxc/JINFENG/http/html/index.html", body);
	
	resp.setBody(body);
}


};
