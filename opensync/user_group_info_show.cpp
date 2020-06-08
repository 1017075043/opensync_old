#include "user_group_info.h"

namespace opensync
{
	void user_group_info::show_all_user_info() //��ʾ�����û���Ϣ
	{
		bimap::left_const_iterator it;
		for (it = user.left.begin(); it != user.left.end(); ++it) 
		{
			out->logs << OUTINFO << it->first << ":" << it->second;
		}
	}
	void user_group_info::show_all_group_info() //��ʾ�����û�����Ϣ
	{
		bimap::left_const_iterator it;
		for (it = group.left.begin(); it != group.left.end(); ++it)
		{
			out->logs << OUTINFO << it->first << ":" << it->second;
		}
	}
}