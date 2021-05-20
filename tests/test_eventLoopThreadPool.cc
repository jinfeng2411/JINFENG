#include "EventLoop.h"
#include "EventLoopThreadPool.h"

int main(){
	JINFENG::EventLoop *loop = new JINFENG::EventLoop();
	JINFENG::EventLoopThreadPool pool(loop);
	pool.start(5);
//	loop->loop();
}
