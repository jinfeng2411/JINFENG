#include "redis.h"
#include <iostream>

namespace JINFENG{

Redis::Redis():cmdTimeout_(){
		
}

//从 conf 中获取 host: ip:port
//		 timeout: cmdTimeout
//	信息
Redis::Redis(std::map<std::string, std::string>& conf) {
    std::string tmp = conf["host"];
    auto pos = tmp.find(":");
    host_ = tmp.substr(0, pos);
    port_ = std::stoi(tmp.substr(pos + 1));

    
    tmp = conf["timeout"];
    uint64_t v = stoi(tmp);

    cmdTimeout_.tv_sec = v / 1000;
    cmdTimeout_.tv_usec = v % 1000 * 1000;
}

//链接数据库
bool Redis::reconnect() {
    return redisReconnect(context_.get());
}

//重连数据库
bool Redis::connect() {
    return connect(host_, port_, 50);
}

//连接指定 IP:port 的redis数据库
bool Redis::connect(const std::string& ip, int port, uint64_t ms){
    host_ = ip;
    port_ = port;
    connectMs_ = ms;
    if(context_) {
        return true;
    }
    timeval tv = {(int)ms / 1000, (int)ms % 1000 * 1000};
    auto c = redisConnectWithTimeout(ip.c_str(), port, tv);
    if(c) {
        if(cmdTimeout_.tv_sec || cmdTimeout_.tv_usec) {
            setTimeout(cmdTimeout_.tv_sec * 1000 + cmdTimeout_.tv_usec / 1000);
        }
        context_.reset(c, redisFree);
        return true;
    }
    return false;
}

bool Redis::setTimeout(uint64_t v) {
    cmdTimeout_.tv_sec = v / 1000;
    cmdTimeout_.tv_usec = v % 1000 * 1000;
    redisSetTimeout(context_.get(), cmdTimeout_);
    return true;
}

Redis::ReplyPtr Redis::cmd(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    ReplyPtr rt = cmd(fmt, ap);
    va_end(ap);
    return rt;
}

Redis::ReplyPtr Redis::cmd(const char* fmt, va_list ap) {
    auto r = (redisReply*)redisCommand(context_.get(), fmt, ap);
    if(!r) {
        return nullptr;
    }
    ReplyPtr rt(r, freeReplyObject);
    if(r->type != REDIS_REPLY_ERROR) {
        return rt;
    }
    return nullptr;
}

Redis::ReplyPtr Redis::cmd(const std::vector<std::string>& argv) {
    std::vector<const char*> v;
    std::vector<size_t> l;
    for(auto& i : argv) {
        v.push_back(i.c_str());
        l.push_back(i.size());
    }

    auto r = (redisReply*)redisCommandArgv(context_.get(), argv.size(), &v[0], &l[0]);
    if(!r) {
        return nullptr;
    }
    ReplyPtr rt(r, freeReplyObject);
    if(r->type != REDIS_REPLY_ERROR) {
        return rt;
    }
    return nullptr;
}

Redis::ReplyPtr Redis::getReply() {
    redisReply* r = nullptr;
    if(redisGetReply(context_.get(), (void**)&r) == REDIS_OK) {
        ReplyPtr rt(r, freeReplyObject);
        return rt;
    }
    return nullptr;
}

int Redis::appendCmd(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int rt = appendCmd(fmt, ap);
    va_end(ap);
    return rt;

}

int Redis::appendCmd(const char* fmt, va_list ap) {
    return redisvAppendCommand(context_.get(), fmt, ap);
}

int Redis::appendCmd(const std::vector<std::string>& argv) {
    std::vector<const char*> v;
    std::vector<size_t> l;
    for(auto& i : argv) {
        v.push_back(i.c_str());
        l.push_back(i.size());
    }
    return redisAppendCommandArgv(context_.get(), argv.size(), &v[0], &l[0]);
}


};
