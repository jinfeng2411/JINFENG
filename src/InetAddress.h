#pragma once

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>

namespace JINFENG{

class IPv4Address{
public:
	IPv4Address(const char* ip="127.0.0.1", uint16_t port=9999):addr_()
	{
		bzero(&addr_, sizeof(addr_));
		addr_.sin_family = AF_INET;
		addr_.sin_port = htons(port);
		int ret = inet_pton(AF_INET, ip, &(addr_.sin_addr));
		assert(ret==1);
	}
	IPv4Address(struct sockaddr_in addr):addr_(addr)
	{
	}

	~IPv4Address()
	{
	}

	const std::string ip() const
	{
		char ip[128];
		const char* ret = inet_ntop(AF_INET, &(addr_.sin_addr), ip, sizeof(ip));
		assert(ret!=NULL);
		return ret;
	}

	uint16_t port() const
	{
		return ntohs(addr_.sin_port);
	}

	struct sockaddr_in& getAddr()
	{
		return addr_;
	}

	void setAddr(const struct sockaddr_in& addr)
	{
		addr_ = addr;
	}	

private:
	sockaddr_in addr_;
};

};
