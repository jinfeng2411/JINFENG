#pragma once

#include <vector>
#include <assert.h>
#include <string>
#include <algorithm>
#include <string.h>
#include <assert.h>
#include <endian.h>

namespace JINFENG{

class Buffer{
public:
	static const size_t kCheapPrepend = 8; //默认前置空间 8 Bytes 
	static const size_t kInitialSize = 1024;  //默认后面存储空间 1024 Bytes 
public:
	Buffer(size_t initialSize = kInitialSize)
		:buffer_(kCheapPrepend + initialSize),
		readerIndex_(kCheapPrepend),
		writerIndex_(kCheapPrepend)
	{
		assert(readableBytes()==0);
		assert(writableBytes()==initialSize);
		assert(prependableBytes()==kCheapPrepend);
	}

	~Buffer()
	{
		
	}

	void swap(Buffer& rhs)
	{
		assert(this!=&rhs);  //防止自身与自身交换
		buffer_.swap(rhs.buffer_);
		std::swap(readerIndex_, rhs.readerIndex_);
		std::swap(writerIndex_, rhs.writerIndex_);
	}
	//可读数据大小
	size_t readableBytes() const
	{
		return writerIndex_-readerIndex_;
	}
	//可写空间大小
	size_t writableBytes() const
	{
		return buffer_.size()-writerIndex_;
	}
	//前置可写大小
	size_t prependableBytes() const
	{
		return readerIndex_;
	}
	//若存在可读数据，则指向第一个可读的数据
	const char* beginRead() const
	{
		return begin()+readerIndex_;
	}
	char* beginRead()
	{
		return begin()+readerIndex_;
	}
	//指向第一个可写的位置
	const char* beginWrite() const
	{
		return begin()+writerIndex_;
	}

	char* beginWrite()
	{
		return begin()+writerIndex_;
	}

	//从可读数据中查找第一个回车换行的位置
	const char* findCRLF() const
	{
		const char kCRLF[] = "\r\n";
		const char* crlf = std::search(beginRead(), beginWrite(), kCRLF, kCRLF+2);
		return crlf==beginWrite() ? NULL:crlf;
	}

	//从可读数据的指定位置查找第一个回车换行的位置
	const char* findCRLF(const char* start) const
	{
		const char kCRLF[] = "\r\n";
		assert(beginRead()<=start);
		assert(start<=beginWrite());
		const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF+2);
		return crlf==beginWrite()? NULL:crlf;
	}

	//从可读数据中查找第一个换行符的位置
	const char* findEOL() const
	{
		const void* eol = memchr(beginRead(), '\n', readableBytes());
		return static_cast<const char*>(eol);
	}

	//从可读数据中的指定位置查找第一个换行符的位置
	const char* findEOL(const char* start) const
	{
		assert(beginRead()<=start);
		assert(start<=beginWrite());
		const void* eol = memchr(start, '\n', beginWrite()-start);
		return static_cast<const char*>(eol);
	}
	
	//清空缓冲区中所有数据
	void retrieveAll()
	{
		readerIndex_ = kCheapPrepend;
		writerIndex_ = kCheapPrepend;
	}

	//清空 len 大小的数据
	void retrieve(size_t len)
	{
		assert(len<=readableBytes());
		if(len<=readableBytes())
		{
			readerIndex_ += len;
		}
		else
		{
			retrieveAll();
		}
	}

	//清空缓冲区end之前的数据
	void retrieveUntil(const char* end)
	{
		assert(beginRead()<=end);
		assert(end<=beginWrite());
		retrieve(end-beginRead());
	}

	void retrieveInt64()
	{
		retrieve(sizeof(int64_t));
	}

	void retrieveInt32()
	{
		retrieve(sizeof(int32_t));
	}

	void retrieveInt16()
	{
		retrieve(sizeof(int16_t));
	}

	void retrieveInt8()
	{
		retrieve(sizeof(int8_t));
	}
	
	//清空 len 字节的数据，并将其转化为 string 返回
	std::string retrieveAsString(size_t len)
	{
		assert(len<=readableBytes());
		std::string result(beginRead(), len);
		if(len==readableBytes())
			retrieveAll();
		else
			retrieve(len);
		return result;
	}

	//清空缓冲区中所有的数据，并将其转化为 string 返回
	std::string retrieveAllAsString()
	{
		return retrieveAsString(readableBytes());
	}

	void hasWritten(size_t len)
	{
		assert(len<=writableBytes());
		writerIndex_ += len;
	}

	void unwrite(size_t len)
	{
		assert(len<=readableBytes());
		writerIndex_ -= len;
	}

	//向缓冲区添加字符串
	void append(const char* data, size_t len)
	{
		ensureWritableBytes(len);
		std::copy(data, data+len, beginWrite());
		hasWritten(len);
	}

	void append(const std::string& data)
	{
		append(data.c_str(), data.size());
	}

	void append(const void* data, size_t len)
	{
		append(static_cast<const char*>(data), len);
	}


	//向缓冲区添加整型数据（转为网络序）
	void appendInt64(int64_t x)
	{
		int64_t be64 = htobe64(x);
		append(&be64, sizeof(be64));
	}
	void appendInt32(int32_t x)
	{
		int32_t be32 = htobe32(x);
		append(&be32, sizeof(be32));
	}
	void appendInt16(int16_t x)
	{
		int16_t be16 = htobe16(x);
		append(&be16, sizeof(be16));
	}
	void appendInt8(int8_t x)
	{
		append(&x, sizeof(x));
	}

	//从缓冲区读取整型数据，从网络序转为本地字节序
	int64_t peekInt64() const
	{
		assert(readableBytes()>=sizeof(int64_t));
		int64_t be64 = 0;
		::memcpy(&be64, beginRead(), sizeof(be64));
		return be64toh(be64);
	}

	int32_t peekInt32() const
	{
		assert(readableBytes()>=sizeof(int32_t));
		int32_t be32 = 0;
		::memcpy(&be32, beginRead(), sizeof(be32));
		return be32toh(be32);
	}

	int16_t peekInt16() const
	{
		assert(readableBytes()>=sizeof(int16_t));
		int16_t be16 = 0;
		::memcpy(&be16, beginRead(), sizeof(be16));
		return be16toh(be16);
	}
	int8_t peekInt8() const
	{
		assert(readableBytes()>=sizeof(int8_t));
		int8_t val = 0;
		::memcpy(&val, beginRead(), sizeof(val));
		return val;
	}

	//读数据的同时，缩小缓冲区
	int64_t readInt64()
	{
		int64_t result = peekInt64();
		retrieveInt64();
		return result;
	}

	int32_t readInt32()
	{
		int32_t result = peekInt32();
		retrieveInt32();
		return result;
	}

	int16_t readInt16()
	{
		int16_t result = peekInt16();
		retrieveInt16();
		return result;
	}

	int8_t readInt8()
	{
		int8_t result = peekInt8();
		retrieveInt8();
		return result;
	}


	//往前置空间中添加数据
	void prepend(const void* data, size_t len)
	{
		assert(len<=prependableBytes());
		readerIndex_ -= len;
		const char* d = static_cast<const char*>(data);
		std::copy(d, d+len, begin()+readerIndex_);
	}

	void prependInt64(int64_t x)
	{
		int64_t be64 = htobe64(x);
		prepend(&be64, sizeof(be64));
	}

	void prependInt32(int32_t x)
	{
		int32_t be32 = htobe32(x);
		prepend(&be32, sizeof(be32));
	}

	void prependInt16(int16_t x)
	{
		int16_t be16 = htobe16(x);
		prepend(&be16, sizeof(be16));
	}

	void prependInt8(int8_t x)
	{
		prepend(&x, sizeof(x));
	}

	void shrink(size_t reserve)
	{
		Buffer other;
		other.ensureWritableBytes(readableBytes()+reserve);
		other.append(retrieveAllAsString());
		swap(other);
	}

	size_t internalCapacity() const
	{
		return buffer_.capacity();
	}

	ssize_t readFd(int fd, int* savedErrno);


	void ensureWritableBytes(size_t len)
	{
		if(writableBytes()<len)
		{
			makeSpace(len);
		}
		assert(writableBytes() >= len);
	}
private:
	char* begin()
	{
		return &*buffer_.begin();
	}

	const char* begin() const
	{
		return &*buffer_.begin();
	}

	void makeSpace(size_t len)
	{
		if(writableBytes()+prependableBytes()<len+kCheapPrepend)
		{
			buffer_.resize(writerIndex_+len);
		}
		else
		{
			assert(kCheapPrepend<readerIndex_);
			size_t readable = readableBytes();
			std::copy(begin()+readerIndex_,
					begin()+writerIndex_,
					begin()+kCheapPrepend);
			readerIndex_ = kCheapPrepend;
			writerIndex_ = readerIndex_ + readable;
			assert(readable==readableBytes());
		}
	}
private:
	std::vector<char> buffer_;
	size_t readerIndex_;
	size_t writerIndex_; //指向第一个可写的位置
};


}
