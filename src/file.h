#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <assert.h>
#include "Logging.h"
#include <string.h>
#include <errno.h>

namespace JINFENG{

/*
 * 一些基本的文件操作
 */
namespace file{

//根据文件名获取文件内容
int getContent(const std::string& filename, std::string& content)
{
	int fd = open(filename.c_str(), O_RDONLY);
	if(fd<0)
	{
		LOG_ERROR<<"open file: "<<filename<<" failed!";
		LOG_ERROR<<strerror(errno);
		assert(false);
	}
	char buf[4096];
	while(1)
	{
		int r = read(fd, buf, sizeof(buf));
		if(r<0){
			LOG_ERROR<<"read file:"<<filename<<" failed!";
			LOG_ERROR<<strerror(errno);
			assert(false);
		}
		else if(r==0)
			break;

		content.append(buf, r);
	}
	return content.size();
}

		

};

};
