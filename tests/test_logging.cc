#include "Logging.h"
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
int main()
{
	//test_logEvent();
	test_logging();
}
