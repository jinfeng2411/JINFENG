#include "Thread.h"
#include "Logging.h"
#include <unistd.h>
#include "Lock.h"

JINFENG::MutexLock mutex_;

int count = 0;

void func()
{
	while(1)
	{
		{
		JINFENG::MutexLockGuard lock(mutex_);
		++count;
		LOG_DEBUG<<count;
		}
		sleep(1);
	}
}

int main()
{
	LOG_DEBUG<<"main";

	JINFENG::Thread t1(func);
	JINFENG::Thread t2(func);
	JINFENG::Thread t3(func);

	t1.start();
	t2.start();
	t3.start();


	t1.join();
	t2.join();
	t3.join();
}
