#pragma once

#include <string>
#include <sys/time.h>
#include <assert.h>
#include <iomanip>
#include <sstream>

namespace JINFENG{

class Timer{
public:


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
};

};
