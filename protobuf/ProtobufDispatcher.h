#pragma once
#include "TcpConnection.h"
#include "ProtobufCodec.h"
#include <map>
#include "Logging.h"

namespace JINFENG{

class ProtobufDispatcher{
public:
	typedef ::google::protobuf::Message Message;
	typedef ::google::protobuf::Descriptor Descriptor;
	typedef TcpConnection::ptr TcpConnectionPtr;
	typedef ProtobufCodec::ProtobufMessageCallback ProtobufMessageCallback;

	//由 ProtobufCodec 中调用
	//当 ProtobufCodec 解析出一条完整的消息时，调用该函数
	void onProtobufMessage(const TcpConnectionPtr& conn, Message* message)
	{
		LOG_TRACE<<"onProtobufMessage";
		const Descriptor* descriptor = message->GetDescriptor();
		if(callbacks_.find(descriptor)!=callbacks_.end())
		{
			callbacks_[descriptor](conn, message);
		}
		else
		{
			LOG_WARN<<"have not set callback for "<<descriptor->full_name();
		}	
	}

	//注册特定类型消息的回调
	template<typename M>
	void registerMessageCallback(
			std::function<void(const TcpConnectionPtr& conn, M* msg)> cb)
	{
		callbacks_[M::descriptor()] = 
			[cb](const TcpConnectionPtr& conn, Message* msg)
			{
				cb(conn, dynamic_cast<M*>(msg));
			};
	}
	
private:
	typedef std::map<const Descriptor*, ProtobufMessageCallback> CallbackMap;
	CallbackMap callbacks_;  //各种消息类型及其对应的回调
};

};
