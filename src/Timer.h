#pragma once

#include <string>
#include <sys/time.h>
#include <assert.h>
#include <iomanip>
#include <sstream>
#include <functional>

namespace JINFENG{


//定时器精度微秒级别
class Timer{
public:
	typedef std::function<void()> Callback;
	Timer(uint64_t expiration, Callback cb, uint64_t interval=0)
		:expiration_(expiration),
		cb_(cb),
		interval_(interval)
	{

	}

	~Timer()
	{

	}

	uint64_t expiration()
	{
		return expiration_;
	}

	uint64_t interval()
	{
		return interval_;
	}

	Callback cb()
	{
		return cb_;
	}

	bool isRepeat()
	{
		return interval_ != 0;
	}

public:
	//当前时间点（微秒）
	static uint64_t nowTimeMicro()
	{

		struct timeval tv;
		int ret = gettimeofday(&tv, NULL);
		assert(ret==0);
		uint64_t now = tv.tv_sec*1000*1000 + tv.tv_usec;
		return now;
	}
	//当前时间点，毫秒
	static uint64_t nowTimeMilli()
	{
		return nowTimeMicro()/1000;
	}

	//写日志时调用
	static std::string getLogTime()
	{
		std::stringstream ss;
		ss.fill('0');
		struct timeval tv;
		int ret = gettimeofday(&tv, NULL);
		assert(ret==0);

		struct tm tm;
		localtime_r(&tv.tv_sec, &tm);
		ss<<std::setw(4)<<tm.tm_year+1900;
		ss<<std::setw(2)<<tm.tm_mon+1;
		ss<<std::setw(2)<<tm.tm_mday;
		ss<<" ";
		ss<<std::setw(2)<<tm.tm_hour;
		ss<<":";
		ss<<std::setw(2)<<tm.tm_min;
		ss<<":";
		ss<<std::setw(2)<<tm.tm_sec;
		ss<<".";
		ss<<tv.tv_usec;
		return ss.str();
	}
private:
	uint64_t expiration_;  //触发时间点
	Callback cb_;  //触发时回调
	uint64_t interval_;  //是否循环
};

};
