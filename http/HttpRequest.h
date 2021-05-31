#pragma once

#include <string>
#include <map>
#include <algorithm>
#include "Logging.h"
#include "Buffer.h"

namespace JINFENG{

class HttpRequest{
public:
	HttpRequest():method_("GET"), uri_("/"), version_("HTTP/1.1")
	{

	}

	std::string method() {return method_;}

	void setMethod(const std::string& method)
	{
		method_ = method;
	}

	std::string uri() {return uri_;}

	void setUri(const std::string& uri)
	{
		uri_ = uri;
	}

	void setVersion(const std::string& version)
	{
		version_ = version;
	}

	void addHeader(const std::string& key, const std::string& val)
	{
		headers_[key] = val;
	}

	void setBody(const std::string& body)
	{
		body_ = body;
	}

	//将 HttpRequest 结构体按 http request 格式写入 buf
	int encode(Buffer& buf) const
	{
		int lastSize = buf.readableBytes();
		//请求行
		buf.append(method_);
		buf.append(" ");
		buf.append(uri_);
		buf.append(" ");
		buf.append(version_);
		buf.append("\r\n");

		//请求头部
		for(auto& header: headers_)
		{
			buf.append(header.first);
			buf.append(":");
			buf.append(header.second);
			buf.append("\r\n");
		}

		//body 字段长度
		buf.append("Content-Length:");
		buf.appendInt32(static_cast<int32_t>(body_.size()));
		buf.append("\r\n\r\n");
		buf.append(body_);
		return buf.readableBytes()-lastSize;
	}

	//解析请求行
	//成功返回 true
	//否则返回 false
	bool decodeRequestLine(const char* begin, const char* end)
	{
		bool succeed = false;
		const char* start = begin;
		const char* space = std::find(start, end, ' ');
		if(space!=end)
		{
			setMethod(std::string(start, space));
			start = space+1;
			space = std::find(start, end, ' ');
			if(space!=end)
			{
				setUri(std::string(start, space));
			}
			start = space+1;
			if(end-start==8 && std::equal(start, end-1, "HTTP/1."))
				succeed = true;
			if(succeed)
			{
				if(*(end-1)=='1')
				{
					setVersion("HTTP/1.1");
				}
				else if(*(end-1)=='0')
				{
					setVersion("HTTP/1.0");
				}
				else
				{
					succeed = false;
				}
			}
		}
		return succeed;
	}

	//从 buf 中解析出 HttpRequest 结构体
	/* 0: buf 中信息还不够
	 * -1: 解析出错
	 *  >0 :成功解析 Request 并消耗的字节数
	 */
	int tryDecode(Buffer& buf)
	{
		int lastSize = buf.readableBytes();
		//解析请求行
		const char* crlf = buf.findCRLF();
		if(crlf==NULL)
			return 0;
		bool ok = decodeRequestLine(buf.beginRead(), crlf);
		if(!ok)
			return -1;
		buf.retrieveUntil(crlf+2);

		//解析头部字段
		while(true){
			crlf = buf.findCRLF();
			if(crlf)
			{
				const char* colon = std::find(static_cast<const char*>(buf.beginRead()), crlf, ':');
				if(colon!=crlf)
				{
					addHeader(std::string(static_cast<const char*>(buf.beginRead()), colon), std::string(colon, crlf));
					buf.retrieveUntil(crlf+2);
				}
				else
				{
					break;
				}
			}
		}

		buf.retrieveUntil(crlf+2);
		//解析body
		LOG_TRACE<<"tryDecode::body";
		return lastSize-buf.readableBytes();
	}
private:
	std::string method_, uri_, version_;
	std::map<std::string, std::string> headers_;
	std::string body_;
};

};
