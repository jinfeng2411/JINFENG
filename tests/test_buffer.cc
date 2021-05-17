#include "Buffer.h"
#include <iostream>
using namespace std;

int main()
{
	JINFENG::Buffer buffer;
	buffer.append("jinfeng");
	buffer.prepend("hello ", 6);
	cout<<buffer.readableBytes()<<endl;
	cout<<buffer.retrieveAllAsString()<<endl;
}
