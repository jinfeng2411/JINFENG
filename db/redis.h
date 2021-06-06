#pragma once
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <sys/time.h>
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace JINFENG{

class Redis{
public:
    typedef std::shared_ptr<Redis> ptr;
    typedef std::shared_ptr<redisReply> ReplyPtr;
    Redis();
    Redis(std::map<std::string, std::string>& conf);

    const std::string& getName() const { return name_;}
    void setName(const std::string& v) { name_ = v;}

    bool reconnect();
    bool connect(const std::string& ip, int port, uint64_t ms = 0);
    bool connect();
    bool setTimeout(uint64_t ms);

    ReplyPtr cmd(const char* fmt, ...);
    ReplyPtr cmd(const char* fmt, va_list ap);
    ReplyPtr cmd(const std::vector<std::string>& argv);

    int appendCmd(const char* fmt, ...);
    int appendCmd(const char* fmt, va_list ap);
    int appendCmd(const std::vector<std::string>& argv);

    ReplyPtr getReply();

private:
    std::string name_;
    std::string host_;
    uint32_t port_;
    uint32_t connectMs_;
    struct timeval cmdTimeout_;
    std::shared_ptr<redisContext> context_;
};


};
