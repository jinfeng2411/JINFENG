#include "Logging.h"
#include "LogFile.h"
#include <iostream>

void test_logEvent()
{
#define xx(type) \
	std::cout<<JINFENG::LogLevel::fromString(type)<<std::endl;

	xx("TRACE");
	xx("DEBUG");
	xx("INFO");
	xx("WARN");
	xx("ERROR");
	xx("FATAL");
#undef xx


#define xx(type) \
	std::cout<<JINFENG::LogLevel::toString(JINFENG::LogLevel::type)<<std::endl;

	xx(TRACE);
	xx(DEBUG);
	xx(INFO);
	xx(WARN);
	xx(ERROR);
	xx(FATAL);
#undef xx
}

void test_logging()
{

#define xx(type) \
	LOG_##type<<#type;

	xx(TRACE);
	xx(DEBUG);
	xx(INFO);
	xx(WARN);
	xx(ERROR);
	xx(FATAL);
#undef xx

}


//test logFile
JINFENG::LogFile logFile("jinfeng", 100);

void output(const std::stringstream& ss)
{
	std::cout<<ss.str().size()<<std::endl;
	logFile.append(ss.str().c_str(), ss.str().size());	
}

void flush()
{
	logFile.flush();
}

void test_logFile()
{
	JINFENG::Logger::setOutputFunc(output);
	JINFENG::Logger::setFlushFunc(flush);
	for(int i=0; i<5; ++i)
	{
		LOG_TRACE<<"test logFile";
		sleep(1);
	}
}

int main()
{
	//test_logEvent();
	//test_logging();
	test_logFile();
}
