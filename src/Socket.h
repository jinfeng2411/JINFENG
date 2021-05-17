#pragma once

#include "InetAddress.h"
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>

namespace JINFENG{

class Socket{
public:
	Socket()
	{
		sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
		assert(sockfd_>=0);
	}
	Socket(int fd):sockfd_(fd)
	{
		assert(sockfd_>=0);
	}
	~Socket()
	{
		int ret = ::close(sockfd_);
		assert(ret==0);
	}

	void bind(IPv4Address addr)
	{
		int ret = ::bind(sockfd_, (struct sockaddr*)&addr.getAddr(), sizeof(addr.getAddr()));
		assert(ret==0);
	}

	void listen()
	{
		int ret = ::listen(sockfd_, 128);
		assert(ret>=0);
	}

	int accept(IPv4Address& peerAddr)
	{
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);
		int connfd = ::accept4(sockfd_, (struct sockaddr*)&peer, &len, SOCK_CLOEXEC|SOCK_NONBLOCK);
		assert(connfd!=-1);
		peerAddr.setAddr(peer);
		return connfd;
	}

	int fd() {return sockfd_;}
public:
	static int setNonBlock(int fd, bool value)
	{
		int flags = fcntl(fd, F_GETFL, 0);
		assert(flags!=-1);
		int ret;
		if(value)
			ret = fcntl(fd, F_SETFL, flags|O_NONBLOCK);
		else
			ret = fcntl(fd, F_SETFL, flags&~O_NONBLOCK);
		assert(ret!=-1);
		return ret;
	}

	static int setReuseAddr(int fd, bool value)
	{
		int flag = value;
		int len = sizeof(flag);
		int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, len);
		assert(ret!=-1);
		return ret;
	}

	static int setReusePort(int fd, bool value)
	{
		int flag = value;
		int len = sizeof(flag);
		int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &flag, len);
		assert(ret!=-1);
		return ret;
	}

	static int setNoDelay(int fd, bool value)
	{
		int flag = value;
		int len = sizeof(flag);
		int ret = setsockopt(fd, SOL_SOCKET, TCP_NODELAY, &flag, len);
		assert(ret!=-1);
		return ret;
	}

private:
	int sockfd_;
};	


};
