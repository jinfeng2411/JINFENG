#include <functional>

namespace JINFENG{

class AutoContext{
public:
	typedef std::function<void()> Task;
	AutoContext():ctx_(nullptr)
	{
	}

	template<class T>
	T& context()
	{
		if(ctx_==nullptr)
		{
			ctx_ = new T();
			delCtx = [this](){delete (T*)ctx_;};
		}
		return *(T*)ctx_;
	}

	~AutoContext()
	{
		if(ctx_!=nullptr)
			delCtx();
	}

private:
	void* ctx_;
	Task delCtx;
};


};
