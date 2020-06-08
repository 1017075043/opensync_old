#include "user_group_info.h"

namespace opensync
{
	bool user_group_info::is_exist_user_name(const string& name) //�ж��Ƿ�����û���
	{
		return user.right.count(name);
	}
	bool user_group_info::is_exist_user_id(const int& id) //�ж��Ƿ�����û�ID
	{
		return user.left.count(id);
	}
	bool user_group_info::is_exist_group_name(const string& name) //�ж��Ƿ�����û�����
	{
		return group.right.count(name);
	}
	bool user_group_info::is_exist_group_id(const int& id) //�ж��Ƿ�����û���ID
	{
		return group.left.count(id);
	}
}