#pragma once

#include <string>
#include <time.h>
#include <unistd.h>
#include <assert.h>
namespace JINFENG{

class LogFile{
public:
	LogFile(const std::string& baseName, 
			size_t rollSize, 
			bool threadSafe = true, 
			int flushInterval = 3);
	~LogFile();

	void append(const char* data, int len);

	void flush();
privat:
	void append_unlock(const char* data, int len);

	void rollFile();

	std::string getLogFileName();

private:
	std::string baseName_;
	size_t rollSize_;
	bool threadSafe_;
	int flushInterval_;
	time_t lastRoll_;
	time_t lastFlush_;
	FILE* fp_;
	size_t written_; 
	const static int kRollPerSeconds_ = 60*60*24;
};



};
