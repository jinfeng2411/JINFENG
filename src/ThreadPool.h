#include "Thread.h"
#include "Lock.h"
#include "Condition.h"
#include <deque>
#include <vector>
#include <functional>
#include <assert.h>

namespace JINFENG{

class ThreadPool{
public:
	typedef std::function<void()> Task;
	ThreadPool();
	~ThreadPool();

	void start(int numThreads);
	void stop();

	void addTask(const Task& task);
private:
	void run();
	Task takeTask();
private:
	bool running_;
	int numThreads_;
	MutexLock mutex_;
	Condition cond_;
	std::vector<Thread*> threads_;
	std::deque<Task> tasks_;
};


};

