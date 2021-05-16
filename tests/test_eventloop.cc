#include "EventLoop.h"
#include "Logging.h"

void func()
{
	LOG_DEBUG<<"in func";
}

int main()
{
	JINFENG::EventLoop loop;
	loop.runAfter(1, func);
	loop.runEvery(3, func);
	loop.loop();
}
