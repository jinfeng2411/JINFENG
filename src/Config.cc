#include "Config.h"
#include <list>
#include "Logging.h"
#include <assert.h>
#include <iostream>

namespace JINFENG{

Config::ConfigVarMap Config::s_datas;

//从 s_datas 中查找对应的变量
ConfigVarBase::ptr Config::LookupBase(const std::string& name)
{
	auto it = s_datas.find(name);
	return it==s_datas.end() ? nullptr : it->second;
}

//找出 Node 中所有的配置变量
static void ListAllMember(const std::string& prefix, 
			const YAML::Node& node,
			std::list<std::pair<std::string, const YAML::Node>>& output)
{
	if(prefix.find_first_not_of("abcdefghijklmnopqrstuvwsyz._0123456789")
			!=std::string::npos)
	{
		LOG_ERROR<<"Config invalid name: "<<prefix;
		return;
	}
	output.push_back(std::make_pair(prefix, node));
	//如果是字典类型，递归解析
	if(node.IsMap()){
		for(auto it=node.begin(); it!=node.end(); ++it)
		{
			std::string nextPrefix;
			if(prefix.empty())
				nextPrefix = it->first.Scalar();
			else
				nextPrefix = prefix+"."+it->first.Scalar();
			ListAllMember(nextPrefix, it->second, output);
		}
	}
}


//前提是该变量已经在 s_datas 中，才会在 LoadFromYaml 中更新
void Config::LoadFromYaml(const YAML::Node& root)
{
	std::list<std::pair<std::string, const YAML::Node>> all_nodes;
	ListAllMember("", root, all_nodes);

	for(auto& item: all_nodes)
	{
		std::string key = item.first;
		if(key.empty())
			continue;
	
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		ConfigVarBase::ptr var = LookupBase(key);

		if(var)
		{
			if(item.second.IsScalar()){
				var->fromString(item.second.Scalar());
			}
			else
			{
				std::stringstream ss;
				ss<<item.second;
				var->fromString(ss.str());
			}
		}
	}

	/*
	for(auto& item: all_nodes)
	{
		std::cout<<"all nodes:";
		std::cout<<item.first<<std::endl;
	}
	*/
}

};
