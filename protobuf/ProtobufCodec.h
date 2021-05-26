#pragma once
#include <functional>
#include "Buffer.h"
#include <assert.h>
#include <google/protobuf/message.h>
#include "TcpConnection.h"
#include "Logging.h"

namespace JINFENG{

class ProtobufCodec
{
public:
	typedef ::google::protobuf::Message Message;
	typedef ::google::protobuf::Descriptor Descriptor;
	typedef TcpConnection::ptr TcpConnectionPtr;
	typedef std::function<void(const TcpConnectionPtr&, Message*)> ProtobufMessageCallback;

	ProtobufCodec(const ProtobufMessageCallback& cb)
		:messageCallback_(cb)
	{

	}

	void onMessage(const TcpConnectionPtr& conn)
	{
		LOG_TRACE<<"ProtobufCodec::onMessage";
		Buffer& buf = conn->inputBuffer();
		if(isCompleted(buf)) //接收到一条完整的消息
		{
			//解析出 message
			Message* message = decode(buf);
			if(!messageCallback_)
				LOG_WARN<<"have not set the callback";
			//调用相应的回调
			messageCallback_(conn, message);
		}
		else
		{
			LOG_TRACE<<"message is not completed, waitting for more data";
		}
	}
	
	// 将 message 打包成固定格式发送：
	void send(const TcpConnectionPtr& conn, const Message& message)
	{
		Buffer buf;
		encode(message, buf);
		conn->send(buf.retrieveAllAsString());
	}

public:
        /*
       将消息编码成固定格式，便于交换
                total_length  (int32_t)
                name_length   (int32_t)
                name          (string)
                message       (string)
         * */
	static void encode(const Message& message, Buffer& buf)
	{
		//保证是 Buffer，因为后面用到了 prepend
		assert(buf.readableBytes()==0);
		std::string name = message.GetDescriptor()->full_name();
		buf.appendInt32(static_cast<int32_t>(name.size()));  //name_length
		buf.append(name);  //name
		std::string msgStr;
		assert(message.SerializeToString(&msgStr));
		buf.append(msgStr); //message
		buf.prependInt32(buf.readableBytes()+4); //total_length
	}

	//判断是否收到一条完整的消息
	static bool isCompleted(const Buffer& buf)
	{
		if(buf.readableBytes()<4) //total_length 字段
			return false;
		//buf 中是否已经有了 total_length的长度
		if(buf.readableBytes()<buf.peekInt32())
			return false;
		return true;
	}

        //根据消息名称新建对应的消息对象
        static Message* createMessage(const std::string& typeName)
        {
                Message* message = nullptr;
                const Descriptor* descriptor =
                       google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
                if(descriptor)
                {
                        const Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
                        if(prototype)
                        {
                                message = prototype->New();
                        }
                }
                else
                {
                        LOG_WARN<<"cannot found the correspond message type: "<<typeName;
                }
                return message;
        }

        //从 buf 中解析出一条消息
        static Message* decode(Buffer& buf)
        {
                int32_t totalLen = buf.readInt32();
                int32_t nameLen = buf.readInt32();
                std::string name = buf.retrieveAsString(nameLen);
                Message* message = createMessage(name);
                std::string msg = buf.retrieveAsString(totalLen-nameLen-sizeof(int32_t)*2);
                assert(message->ParseFromString(msg));
                return message;
        }

private:
	ProtobufMessageCallback messageCallback_;

};

};
