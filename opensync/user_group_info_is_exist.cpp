#include "user_group_info.h"

namespace opensync
{
	bool user_group_info::is_exist_user_name(const string& name) //判断是否存在用户名
	{
		return user.right.count(name);
	}
	bool user_group_info::is_exist_user_id(const int& id) //判断是否存在用户ID
	{
		return user.left.count(id);
	}
	bool user_group_info::is_exist_group_name(const string& name) //判断是否存在用户组名
	{
		return group.right.count(name);
	}
	bool user_group_info::is_exist_group_id(const int& id) //判断是否存在用户组ID
	{
		return group.left.count(id);
	}
}