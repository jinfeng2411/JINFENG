#include "LogFile.h"
#include <iomanip>
#include <sstream>

namespace JINFENG{

LogFile::LogFile(const std::string& baseName,
		size_t rollSize,
		bool threadSafe,
		int flushInterval)
	:baseName_(baseName),
	rollSize_(rollSize),
	threadSafe_(threadSafe),
	flushInterval_(flushInterval),
	lastRoll_((time(NULL))/kRollPerSeconds_),
	lastFlush_(time(NULL)),
	written_(0),
	mutex_()
{
	assert(baseName_.find('/')==std::string::npos);
	std::string filename = getLogFileName();
	fp_ = fopen(filename.c_str(), "ae");
	assert(fp_!=NULL);
}

LogFile::~LogFile()
{
	int ret = fclose(fp_);
	assert(ret==0);
}

void LogFile::append(const char* data, int len)
{
	if(threadSafe_)
	{
		MutexLockGuard lock(mutex_);
		append_unlock(data, len);
	}
	else
	{
		append_unlock(data, len);
	}
}

void LogFile::flush()
{
	if(threadSafe_)
	{
		MutexLockGuard lock(mutex_);
		int ret = fflush(fp_);
		assert(ret==0);
	}
	else
	{
		int ret = fflush(fp_);
		assert(ret==0);
	}
}


void LogFile::append_unlock(const char* data, int len)
{ 
	size_t ret = fwrite(data, 1, len, fp_);
	assert(ret==len);

	written_ += len;
	//日志文件超过一定大小，切换文件
	if(written_>rollSize_)
	{
		rollFile();
	}
	time_t now = ::time(NULL);
	//时间超过一定的间隔，切换文件
	if(now/kRollPerSeconds_ > lastRoll_)
	{
		rollFile();
		lastRoll_ = now/kRollPerSeconds_;
		lastFlush_ = now;
	}
	//每隔一定的时间刷新
	else if(now-lastFlush_>flushInterval_)
	{
		lastFlush_ = now;
		flush();
	}
}

void LogFile::rollFile()
{
	int ret = fclose(fp_); //关闭旧日志文件
	std::string logFileName = getLogFileName();
	fp_ = fopen(logFileName.c_str(), "ae"); //创建新日志文件
	assert(fp_!=NULL);
	written_ = 0;
}


std::string LogFile::getLogFileName()
{
	std::stringstream ss;
	ss<<baseName_<<".";
	time_t now = time(NULL);
	struct tm tm;
	localtime_r(&now, &tm);
	ss.fill('0');
	ss<<tm.tm_year+1900;
	ss<<std::setw(2)<<tm.tm_mon+1;
	ss<<std::setw(2)<<tm.tm_mday;
	ss<<"-";
	ss<<std::setw(2)<<tm.tm_hour;
	ss<<std::setw(2)<<tm.tm_min;
	ss<<std::setw(2)<<tm.tm_sec;
	ss<<".";
	ss<<getpid();
	ss<<".log";
	return ss.str();
}



};
