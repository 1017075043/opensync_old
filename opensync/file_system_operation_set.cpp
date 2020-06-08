#include "file_system_operation.h"
namespace opensync
{
	bool file_system_operation::set_file_permissions(const string& file_path, const perms& permissions)//设置文件file_permissions
	{
		if (!file_info->is_exist_file_permissions(permissions))
		{
			out->logs << OUTERROR << permissions << "is not exist file permissions";
			return false;
		}
		if (file_info->data[file_path].permissions == permissions)
		{
			return true;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			try
			{
				if (chmod(file_path.c_str(), permissions) == -1)
				{
					throw exception() << err_str(file_path + " " + strerror(errno));
				}
				file_info->data[file_path].permissions = permissions;
				file_info->data[file_path].permissions_name = file_info->transfrom_file_permissions(file_info->data[file_path].permissions);
			}
			catch (exception& e)
			{
				out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			}
		}
		out->logs << OUTDEBUG << "set_file_permissions " << file_path << ":" << file_info->data[file_path].permissions;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::set_file_permissions(const string& file_path, const string& permissions_name)//设置文件file_permissions_name
	{
		if (!file_info->is_exist_file_permissions(permissions_name))
		{
			out->logs << OUTERROR << permissions_name << "is not exist file permissions";
			return false;
		}
		if (file_info->data[file_path].permissions_name == permissions_name)
		{
			return true;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			try
			{
				if (chmod(file_path.c_str(), (perms)file_info->transfrom_file_permissions(permissions_name)) == -1)
				{
					throw exception() << err_str(file_path + " " + strerror(errno));
				}
				file_info->data[file_path].permissions = (perms)file_info->transfrom_file_permissions(permissions_name);
				file_info->data[file_path].permissions_name = permissions_name;
			}
			catch (exception& e)
			{
				out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			}
		}
		out->logs << OUTDEBUG << "set_file_permissions " << file_path << ":" << file_info->data[file_path].permissions_name;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::set_file_last_write_time(const string& file_path, const time_t& last_write_time) //设置文件file_last_write_time
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			boost::filesystem::last_write_time(file_info->data[file_path].file_path, last_write_time);
			file_info->data[file_path].last_write_time = last_write_time;
			file_info->data[file_path].last_write_time_s = boost::posix_time::to_iso_extended_string(boost::posix_time::from_time_t(file_info->data[file_path].last_write_time));
		}
		out->logs << OUTDEBUG << "set_file_last_write_time " << file_path << ":" << file_info->data[file_path].last_write_time;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::set_file_last_write_time_s(const string& file_path, const string& last_write_time_s) //设置文件file_last_write_time_s
	{
		return set_file_last_write_time(file_path, opensync::string_operation::to_date(last_write_time_s, "%d-%d-%dT%d:%d:%d"));
	}
	bool file_system_operation::set_file_user(const string& file_path, const int& user)//设置文件file_permissions
	{
		if (!user_group->is_exist_user_id(user))
		{
			out->logs << OUTERROR << user << "is not exist user id";
			return false;
		}
		if (file_info->data[file_path].user == user)
		{
			return true;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			try
			{
				if (chown(file_path.c_str(), (uid_t)user, (uid_t)file_info->data[file_path].group) == -1)
				{
					throw exception() << err_str(file_path + " " + strerror(errno));
				}
				file_info->data[file_path].user = user;
				file_info->data[file_path].user_name = user_group->get_user_name(file_info->data[file_path].user);
			}
			catch (exception& e)
			{
				out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			}
		}
		out->logs << OUTDEBUG << "set_file_user " << file_path << ":" << file_info->data[file_path].user;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::set_file_user(const string& file_path, const string& user_name)//设置文件file_permissions_name
	{
		if (!user_group->is_exist_user_name(user_name))
		{
			out->logs << OUTERROR << user_name << "is not exist user name";
			return false;
		}
		if (file_info->data[file_path].user_name == user_name)
		{
			return true;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			try
			{
				if (chown(file_path.c_str(), (uid_t)user_group->get_user_id(user_name), (uid_t)file_info->data[file_path].group) == -1)
				{
					throw exception() << err_str(file_path + " " + strerror(errno));
				}
				file_info->data[file_path].user = user_group->get_user_id(user_name);
				file_info->data[file_path].user_name = user_name;
			}
			catch (exception& e)
			{
				out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			}
		}
		out->logs << OUTDEBUG << "set_file_user " << file_path << ":" << file_info->data[file_path].user_name;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::set_file_group(const string& file_path, const int& group)//设置文件file_user
	{
		if (!user_group->is_exist_group_id(group))
		{
			out->logs << OUTERROR << group << "is not exist group id";
			return false;
		}
		if (file_info->data[file_path].group == group)
		{
			return true;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			try
			{
				if (chown(file_path.c_str(), (uid_t)file_info->data[file_path].user, (uid_t)group) == -1)
				{
					throw exception() << err_str(file_path + " " + strerror(errno));
				}
				file_info->data[file_path].group = group;
				file_info->data[file_path].group_name = user_group->get_user_name(file_info->data[file_path].group);
			}
			catch (exception& e)
			{
				out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			}
		}
		out->logs << OUTDEBUG << "set_file_group " << file_path << ":" << file_info->data[file_path].group;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::set_file_group(const string& file_path, const string& group_name)//设置文件file_group_name
	{
		if (!user_group->is_exist_group_name(group_name))
		{
			out->logs << OUTERROR << group_name << "is not exist group name";
			return false;
		}
		if (file_info->data[file_path].group_name == group_name)
		{
			return true;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			try
			{
				if (chown(file_path.c_str(), (uid_t)file_info->data[file_path].user, (uid_t)user_group->get_group_id(group_name)) == -1)
				{
					throw exception() << err_str(file_path + " " + strerror(errno));
				}
				file_info->data[file_path].group = user_group->get_group_id(group_name);
				file_info->data[file_path].group_name = group_name;
			}
			catch (exception& e)
			{
				out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			}
		}
		out->logs << OUTDEBUG << "set_file_group " << file_path << ":" << file_info->data[file_path].group_name;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::set_file_user_group(const string& file_path, const int& user, const int& group)//设置文件file_user和file_user_name
	{
		if (!user_group->is_exist_user_id(user))
		{
			out->logs << OUTERROR << user << "is not exist user id";
			return false;
		}
		if (!user_group->is_exist_group_id(group))
		{
			out->logs << OUTERROR << group << "is not exist group id";
			return false;
		}
		if (file_info->data[file_path].user == user && file_info->data[file_path].group == group)
		{
			return true;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			try
			{
				if (chown(file_path.c_str(), (uid_t)user, (uid_t)group) == -1)
				{
					throw exception() << err_str(file_path + " " + strerror(errno));
				}
				file_info->data[file_path].user = user;
				file_info->data[file_path].user_name = user_group->get_user_name(user);
				file_info->data[file_path].group = group;
				file_info->data[file_path].group_name = user_group->get_user_name(group);
			}
			catch (exception& e)
			{
				out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			}
		}
		out->logs << OUTDEBUG << "set_file_user " << file_path << ":" << file_info->data[file_path].user << "," << file_info->data[file_path].group;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::set_file_user_group(const string& file_path, const string& user_name, const string& group_name)//设置文件file_user_name和file_user_name
	{
		if (!user_group->is_exist_user_name(user_name))
		{
			out->logs << OUTERROR << user_name << "is not exist user name";
			return false;
		}
		if (!user_group->is_exist_group_name(group_name))
		{
			out->logs << OUTERROR << group_name << "is not exist group name";
			return false;
		}
		if (file_info->data[file_path].user_name == user_name && file_info->data[file_path].group_name == group_name)
		{
			return true;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			try
			{
				if (chown(file_path.c_str(), (uid_t)user_group->get_group_id(user_name), (uid_t)user_group->get_group_id(group_name)) == -1)
				{
					throw exception() << err_str(file_path + " " + strerror(errno));
				}
				file_info->data[file_path].user = user_group->get_group_id(user_name);
				file_info->data[file_path].user_name = user_name;
				file_info->data[file_path].group = user_group->get_group_id(group_name);
				file_info->data[file_path].group_name = group_name;
			}
			catch (exception& e)
			{
				out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			}
		}
		out->logs << OUTDEBUG << "set_file_user " << file_path << ":" << file_info->data[file_path].user_name << "," << file_info->data[file_path].group_name;
		return file_info->data[file_path].status;
	}

}