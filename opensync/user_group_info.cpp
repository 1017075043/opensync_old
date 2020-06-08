#include "user_group_info.h"

namespace opensync
{
	user_group_info* user_group_info::instance = nullptr;

	user_group_info::user_group_info()
	{
		init_system_all_user_group_info();
	}
	user_group_info::~user_group_info()
	{
	}

	user_group_info* user_group_info::get_instance()
	{
		return instance;
	}
	user_group_info* user_group_info::init_instance()
	{
		if (instance == nullptr) {
			instance = new user_group_info();
		}
		return instance;
	}
	void user_group_info::destory()
	{
		if (instance != nullptr) 
		{
			delete instance;
			instance = nullptr;
		}
	}

	bool user_group_info::init_system_all_user_group_info() //获取系统所有用户、用户组信息
	{
		if (!init_system_all_user_info() || init_system_all_group_info())
		{
			return false;
		}
		return true;
	}
	bool user_group_info::init_system_all_user_info() //获取系统所有用户信息
	{
		try
		{
			FILE* stream = fopen(passwd_file.c_str(), "r");
			if (stream == nullptr)
			{
				throw exception() << err_str(passwd_file + " " + strerror(errno));
			}
			struct passwd* user_info;
			while ((user_info = fgetpwent(stream)) != 0)
			{
				user.left.insert(make_pair((int)user_info->pw_uid, user_info->pw_name));
				//out->logs << OUTDEBUG << "add, user_id:" << (int)user_info->pw_uid << ", user_name:" << user_info->pw_name;
			}
			fclose(stream);
			return true;
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return false;
		}
	}
	bool user_group_info::init_system_all_group_info() //获取系统所有组用户信息
	{
		try
		{
			FILE* stream = fopen(group_file.c_str(), "r");
			if (stream == nullptr)
			{
				throw exception() << err_str(group_file + " " + strerror(errno));
			}
			struct group* group_info;
			while ((group_info = fgetgrent(stream)) != 0)
			{
				group.insert(bimap::value_type((int)group_info->gr_gid, group_info->gr_name));
				//out->logs << OUTDEBUG << "add, group_id:" << (int)group_info->gr_gid << ", group_name:" << group_info->gr_name;
			}
			fclose(stream);
			return true;
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return false;
		}
	}
}