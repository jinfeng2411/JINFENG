#include "Config.h"
#include "Logging.h"
#include <iostream>
using namespace std;


void test_config()
{
	JINFENG::ConfigVar<std::string>::ptr name
		= JINFENG::Config::Lookup("server1.name", std::string(""), "serverName");
	JINFENG::ConfigVar<std::string>::ptr ip
		= JINFENG::Config::Lookup("server1.ip", std::string(""), "ipAddress");
	JINFENG::ConfigVar<int>::ptr port
		= JINFENG::Config::Lookup("server1.port", 0, "listenport");

	cout<<name->name()<<":"<<name->value()<<endl;
	cout<<ip->name()<<":"<<ip->value()<<endl;
	cout<<port->name()<<":"<<port->value()<<endl;

	YAML::Node root = YAML::LoadFile("conf.yaml");
	JINFENG::Config::LoadFromYaml(root);  //从配置文件中更新 配置变量的信息
	cout<<name->name()<<":"<<name->value()<<endl;
	cout<<ip->name()<<":"<<ip->value()<<endl;
	cout<<port->name()<<":"<<port->value()<<endl;
}


void print_yaml(const YAML::Node& node)
{
	if(node.IsScalar())
	{
		cout<<node.Scalar()<<endl;
	}
	else if(node.IsNull())
	{
		cout<<"NULL";
	}
	else if(node.IsMap())
	{
		for(auto it = node.begin(); it!=node.end(); ++it)
		{
			cout<<it->first;
			print_yaml(it->second);
		}
	}
	else if(node.IsSequence())
	{
		for(auto it = node.begin(); it!=node.end(); ++it)
			print_yaml(*it);
	}
}

int main()
{
	test_config();	
}
