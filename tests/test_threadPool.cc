#include "ThreadPool.h"
#include "Logging.h"
#include <string>
#include <iostream>

int main()
{
	typedef std::function<void()> Task;
	JINFENG::ThreadPool threadPool;
	threadPool.start(5);

	
	for(int i=100; i>=1; --i)
	{
		
		auto t = [=]{
			LOG_TRACE<<("task_"+std::to_string(i));
		};
		
		threadPool.addTask(t);
	}

	sleep(5);
	threadPool.stop();
}
