#pragma once

#include <string>
#include <sstream>
#include <assert.h>
#include <functional>
#include <stdio.h>
#include "utils.h"
#include "Timer.h"

#define LOG_TRACE \
	JINFENG::Logger(JINFENG::LogLevel::TRACE, __FILE__, __LINE__).stream()
#define LOG_DEBUG \
	JINFENG::Logger(JINFENG::LogLevel::DEBUG, __FILE__, __LINE__).stream()
#define LOG_INFO \
	JINFENG::Logger(JINFENG::LogLevel::INFO, __FILE__, __LINE__).stream()
#define LOG_WARN \
	JINFENG::Logger(JINFENG::LogLevel::WARN, __FILE__, __LINE__).stream()
#define LOG_ERROR \
	JINFENG::Logger(JINFENG::LogLevel::ERROR, __FILE__, __LINE__).stream()
#define LOG_FATAL \
	JINFENG::Logger(JINFENG::LogLevel::FATAL, __FILE__, __LINE__).stream()

namespace JINFENG{

class LogLevel{
public:
	enum Level{
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL
	};

	static std::string toString(const LogLevel::Level& level);
	static LogLevel::Level fromString(const std::string& str);
};


typedef std::function<void(const std::stringstream&)> OutputFunc;
typedef std::function<void()> FlushFunc;

void defaultOutput(const std::stringstream& stream); //默认日志输出函数
void defaultFlush(); //默认日志刷新函数

class Logger{
public:
	Logger(const LogLevel::Level& level, const char* fileName, const uint64_t& line);
	~Logger();

	std::stringstream& stream() {return stream_;}

	void setOutputFunc(const OutputFunc& func);

	void setFlushFunc(const FlushFunc& func);

private:
	const char* fileName_;
	uint64_t line_;
	std::stringstream stream_;
	static OutputFunc outputFunc_;
	static FlushFunc flushFunc_;
};

};
