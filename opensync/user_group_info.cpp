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

	bool user_group_info::init_system_all_user_group_info() //��ȡϵͳ�����û����û�����Ϣ
	{
		if (!init_system_all_user_info() || init_system_all_group_info())
		{
			return false;
		}
		return true;
	}
	bool user_group_info::init_system_all_user_info() //��ȡϵͳ�����û���Ϣ
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
	bool user_group_info::init_system_all_group_info() //��ȡϵͳ�������û���Ϣ
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

	string user_group_info::get_user_name(const int& id) //��ȡ�û���
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
	int user_group_info::get_user_id(const string& name) //��ȡ�û�ID
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

	string user_group_info::get_group_name(const int& id) //��ȡ�û�����
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
	int user_group_info::get_group_id(const string& name) //��ȡ�û���ID
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