#include "file_system_operation.h"
namespace opensync
{
	void file_system_operation::update_file_info(const string& file_path) //更新文件信息
	{
		init_file_info(file_path);
	}
	bool file_system_operation::update_file_size(const string& file_path, const long& file_size) //更新文件file_size
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].file_size = file_size;
		}
		out->logs << OUTDEBUG << "update_file_size " << file_path << ":" << file_info->data[file_path].file_size;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_type(const string& file_path, const file_type& type) //更新文件file_type
	{
		if (!file_info->is_exist_file_type(type))
		{
			return false;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].type = type;
			file_info->data[file_path].type_name = file_info->transfrom_file_type(file_info->data[file_path].type);
		}
		out->logs << OUTDEBUG << "update_file_type " << file_path << ":" << file_info->data[file_path].type;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_type_name(const string& file_path, const string& type_name) //更新文件file_type_name
	{
		if (!file_info->is_exist_file_type(type_name))
		{
			return false;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].type = (file_type)file_info->transfrom_file_type(file_info->data[file_path].type_name);
			file_info->data[file_path].type_name = type_name;
		}
		out->logs << OUTDEBUG << "update_file_type_name " << file_path << ":" << file_info->data[file_path].type_name;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_permissions(const string& file_path, const perms& permissions) //更新文件file_permissions
	{
		if (!file_info->is_exist_file_permissions(permissions))
		{
			return false;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].permissions = permissions;
			file_info->data[file_path].type_name = file_info->transfrom_file_permissions(file_info->data[file_path].permissions);
		}
		out->logs << OUTDEBUG << "update_file_permissions " << file_path << ":" << file_info->data[file_path].permissions;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_permissions_name(const string& file_path, const string& permissions_name) //更新文件file_permissions_name
	{
		if (!file_info->is_exist_file_permissions(permissions_name))
		{
			return false;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].permissions = (perms)file_info->transfrom_file_type(permissions_name);
			file_info->data[file_path].type_name = permissions_name;
		}
		out->logs << OUTDEBUG << "update_file_permissions_name " << file_path << ":" << file_info->data[file_path].permissions_name;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_last_write_time(const string& file_path, const time_t& last_write_time) //更新文件file_last_write_time
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].last_write_time = last_write_time;
			file_info->data[file_path].last_write_time_s = boost::posix_time::to_iso_extended_string(boost::posix_time::from_time_t(file_info->data[file_path].last_write_time));
		}
		out->logs << OUTDEBUG << "update_file_last_write_time " << file_path << ":" << file_info->data[file_path].last_write_time;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_last_write_time_s(const string& file_path, const string& last_write_time_s) //更新文件file_last_write_time_s
	{
		return update_file_last_write_time(file_path, opensync::string_operation::to_date(last_write_time_s, "%d-%d-%dT%d:%d:%d"));
	}
	bool file_system_operation::update_file_user(const string& file_path, const int& user) //更新文件file_user
	{
		if (!user_group->is_exist_group_id(user))
		{
			return false;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].user = user;
			file_info->data[file_path].user_name = user_group->get_user_name(file_info->data[file_path].user);
		}
		out->logs << OUTDEBUG << "update_file_user " << file_path << ":" << file_info->data[file_path].user;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_user_name(const string& file_path, const string& user_name) //更新文件file_user_name
	{
		if (!user_group->is_exist_group_name(user_name))
		{
			return false;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].user = user_group->get_user_id(user_name);
			file_info->data[file_path].user_name = user_name;
		}
		out->logs << OUTDEBUG << "update_file_user_name " << file_path << ":" << file_info->data[file_path].user_name;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_group(const string& file_path, const int& group) //更新文件file_group
	{
		if (!user_group->is_exist_group_id(group))
		{
			return false;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].group = group;
			file_info->data[file_path].group_name = user_group->get_group_name(file_info->data[file_path].group);
		}
		out->logs << OUTDEBUG << "update_file_group " << file_path << ":" << file_info->data[file_path].group;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_group_name(const string& file_path, const string& group_name) //更新文件file_group_name
	{
		if (!user_group->is_exist_group_name(group_name))
		{
			return false;
		}
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].group = user_group->get_group_id(group_name);
			file_info->data[file_path].group_name = group_name;
		}
		out->logs << OUTDEBUG << "update_file_group_name " << file_path << ":" << file_info->data[file_path].group_name;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_hash(const string& file_path, const string& file_hash) //更新文件file_hash
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].file_hash = file_hash;
		}
		out->logs << OUTDEBUG << "update_file_hash " << file_path << ":" << file_info->data[file_path].file_hash;
		return file_info->data[file_path].status;
	}
	bool file_system_operation::update_file_status(const string& file_path, const bool& status) //更新文件file_status
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		else if (file_info->data[file_path].status)
		{
			file_info->data[file_path].status = status;
		}
		out->logs << OUTDEBUG << "update_file_status " << file_path << ":" << file_info->data[file_path].status;
		return file_info->data[file_path].status;
	}
}