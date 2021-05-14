#include "Logging.h"
#include <iostream>
#include <libgen.h>
#include <iomanip>

namespace JINFENG
{


std::string LogLevel::toString(const LogLevel::Level& level)
{
#define xx(type) \
	if(level==LogLevel::type) \
		return #type; 

	xx(TRACE);
	xx(DEBUG);
	xx(INFO);
	xx(WARN);
	xx(ERROR);
	xx(FATAL);
#undef xx
	assert(false);  //正常情况下不可能运行到这儿
}


LogLevel::Level LogLevel::fromString(const std::string& str)
{
#define xx(type) \
	if(str==#type) \
		return LogLevel::type;

	xx(TRACE);
	xx(DEBUG);
	xx(INFO);
	xx(WARN);
	xx(ERROR);
	xx(FATAL);
#undef xx

	assert(false); //正常情况不可能运行到这儿
}


void defaultOutput(const std::stringstream& stream)
{
        ssize_t n = fwrite(stream.str().c_str(), stream.str().size(), 1, stdout);
}

void defaultFlush()
{
        int n = fflush(stdout);
        assert(n==0);
}


OutputFunc Logger::outputFunc_ = defaultOutput;
FlushFunc Logger::flushFunc_ = defaultFlush;

Logger::Logger(const LogLevel::Level& level, const char* fileName, const uint64_t& line)
	:level_(level),
	fileName_(fileName),
	line_(line)
{
	stream_<<Timer::getLogTime()<<" ";
	stream_<<getTid()<<" ";
	stream_.fill(' ');
	stream_<<std::left<<std::setw(6)<<LogLevel::toString(level);
}

Logger::~Logger()
{
	stream_<<" - "<<basename(const_cast<char*>(fileName_))<<" ";
	stream_<<line_;
	stream_<<"\n";
	outputFunc_(stream_);
	if(level_==LogLevel::FATAL)
		flushFunc_();
}

void Logger::setOutputFunc(const OutputFunc& func)
{
        outputFunc_ = func;
}

void Logger::setFlushFunc(const FlushFunc& func)
{
       	flushFunc_ = func;
}




};
