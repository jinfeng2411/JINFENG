#include "redis.h"
#include <iostream>

int main()
{
	JINFENG::Redis redis;
	if(redis.connect("127.0.0.1", 6379))
		std::cout<<"connected to redis successfully!\n";

	JINFENG::Redis::ReplyPtr ptr = redis.cmd("set jinfeng yulu");
	std::cout<<ptr->str<<std::endl;
	ptr = redis.cmd("get jinfeng");
	std::cout<<ptr->str<<std::endl;

	std::vector<std::string> argv1 = {"set", "li", "zhenjing"};
	ptr = redis.cmd(argv1);
	std::cout<<ptr->str<<std::endl;
	std::vector<std::string> argv2 = {"get", "li"};
	ptr = redis.cmd(argv2);
	std::cout<<ptr->str<<std::endl;


	//pipeline 命令，不会立即发送，先缓存到 context_ 对象中
	int rt = redis.appendCmd("set shi wanting");
	rt = redis.appendCmd("set lei wangran");
	std::cout<<rt<<std::endl;
	//此时会发送前面两条命令，并返回第一条命令的执行结果 OK
	ptr = redis.getReply();
	std::cout<<ptr->str<<std::endl;

	//第二条命令的执行结果 OK
	ptr = redis.getReply();
	std::cout<<ptr->str<<std::endl;


	ptr = redis.cmd("get shi");
	std::cout<<ptr->str<<std::endl;

}
