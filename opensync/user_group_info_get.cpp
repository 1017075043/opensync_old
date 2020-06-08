#include "user_group_info.h"

namespace opensync
{
	string user_group_info::get_user_name(const int& id) //获取用户名
	{
		bimap::left_const_iterator it;
		it = user.left.find(id);
		if (it != user.left.end())
		{
			//out->logs << OUTDEBUG << id << ":" << it->second;
			return it->second;
		}
		out->logs << OUTWARN << id << " is not exist";
		return "";
	}
	int user_group_info::get_user_id(const string& name) //获取用户ID
	{
		bimap::right_const_iterator it;
		it = user.right.find(name);
		if (it != user.right.end())
		{
			//out->logs << OUTDEBUG << name << ":" << it->second;
			return it->second;
		}
		out->logs << OUTWARN << name << " is not exist";
		return -1;
	}
	string user_group_info::get_group_name(const int& id) //获取用户组名
	{
		bimap::left_const_iterator it;
		it = group.left.find(id);
		if (it != group.left.end())
		{
			//out->logs << OUTDEBUG << id << ":" << it->second;
			return it->second;
		}
		out->logs << OUTWARN << id << " is not exist";
		return "";
	}
	int user_group_info::get_group_id(const string& name) //获取用户组ID
	{
		bimap::right_const_iterator it;
		it = group.right.find(name);
		if (it != group.right.end())
		{
			//out->logs << OUTDEBUG << name << ":" << it->second;
			return it->second;
		}
		out->logs << OUTWARN << name << " is not exist";
		return -1;
	}
}