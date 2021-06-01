#pragma once

#include <string>
#include <typeinfo>
#include <map>
#include <memory>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <boost/lexical_cast.hpp>
#include "Logging.h"

namespace JINFENG{

//配置变量基类
class ConfigVarBase{
public:
	typedef std::shared_ptr<ConfigVarBase> ptr;
	ConfigVarBase(const std::string& name, const std::string& description="")
		:name_(name),
		description_(description)
	{
		std::transform(name_.begin(), name_.end(), name_.begin(), ::tolower);
	}

	virtual ~ConfigVarBase(){}

	const std::string& name() const {return name_;}

	const std::string& description() const {return description_;}

	virtual std::string toString() = 0;

	virtual bool fromString(const std::string& val) = 0;
protected:
	std::string name_;  //配置变量名
	std::string description_;  //配置变量描述
};


//配置变量类
template<class T>
class ConfigVar: public ConfigVarBase{
public:
	typedef std::shared_ptr<ConfigVar> ptr;
	ConfigVar(const std::string& name, const T& default_value, const std::string& description=""): ConfigVarBase(name, description), val_(default_value)
	{

	}
	//配置变量的值 转 string
	std::string toString() override{
		try{
			return boost::lexical_cast<std::string>(val_);
		}catch(std::exception& e)
		{
			LOG_ERROR<<"ConfigVar::toString :"<<e.what()<<" convert "<<typeid(val_).name()<<"to string";
		}
		return "";
	}
	//从 string 类型的数据获取配置变量的值
	bool fromString(const std::string& val) override
	{
		try{
			val_ = boost::lexical_cast<T>(val);
		}catch(std::exception& e)
		{
			LOG_ERROR<<"ConfigVar::fromString: "<<e.what()<<" convert string to "<<typeid(val_).name();
		}
		return false;
	}

	//返回配置变量的值
	const T value() const
	{
		return val_;
	}

	//设置配置变量
	void setValue(const T& v)
	{
		val_ = v;
	}

private:
	T val_; //配置变量值
};

//配置类
class Config{
public:
	//配置变量集合
	typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

	//返回名为 name 的配置变量
	//若没有找到，则根据默认值构建，并加入配置变量集合中
	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name,
			const T& default_value, const std::string& description="")
	{
		auto tmp = Lookup<T>(name);
		if(tmp)
		{
			LOG_TRACE<<"Lookup name="<<name<<" exists";
			return tmp;
		}

		if(name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678")
				!=std::string::npos)
		{
			LOG_ERROR<<"Loopup name invalid:"<<name;
			return tmp;
		}

		typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
		s_datas[name] = v;
		return v;
	}
	
	//返回名为 name 的配置变量
	//若不存在，则返回 nullptr
	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name)
	{
		auto it = s_datas.find(name);
		if(it==s_datas.end())
			return nullptr;
		return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
	}

	static ConfigVarBase::ptr LookupBase(const std::string& name);

	//从 yaml 配置文件中更新 配置变量集合中的配置变量的值
	static void LoadFromYaml(const YAML::Node& root);

private:
	static ConfigVarMap s_datas;
};

};
