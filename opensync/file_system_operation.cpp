#include "file_system_operation.h"
namespace opensync
{
	file_system_operation::file_system_operation()
	{
	}
	file_system_operation::~file_system_operation()
	{
	}
	opensync::file_attribute file_system_operation::get_file_info(const string& file_path) //获取一个文件属性信息
	{
		try
		{
			out->logs << OUTINFO << "create file info:" << file_path;

			boost::filesystem::path p(file_path);
			boost::filesystem::file_status s = boost::filesystem::status(p);

			file_info->data[file_path].file_path = p;
			//out->logs << OUTDEBUG << file_path << ", attribute.file_path=" << file_info->data[file_path].file_path;
			file_info->data[file_path].file_size = (long)boost::filesystem::file_size(p);
			//out->logs << OUTDEBUG << file_path << ", attribute.file_size=" << file_info->data[file_path].file_size;
			file_info->data[file_path].type = s.type();
			//out->logs << OUTDEBUG << file_path << ", attribute.type=" << file_info->data[file_path].type;
			file_info->data[file_path].type_name = file_info->transfrom_file_type((int)s.type());
			//out->logs << OUTDEBUG << file_path << ", attribute.type_name=" << file_info->data[file_path].type_name;
			file_info->data[file_path].permissions = s.permissions();
			//out->logs << OUTDEBUG << file_path << ", attribute.permissions=" << file_info->data[file_path].permissions;
			file_info->data[file_path].permissions_name = file_info->transfrom_file_permissions((int)s.permissions());
			//out->logs << OUTDEBUG << file_path << ", attribute.permissions_name=" << file_info->data[file_path].permissions_name;
			file_info->data[file_path].last_write_time = boost::filesystem::last_write_time(p);
			//out->logs << OUTDEBUG << file_path << ", attribute.last_write_time=" << file_info->data[file_path].last_write_time;
			file_info->data[file_path].last_write_time_s = boost::posix_time::to_iso_extended_string(boost::posix_time::from_time_t(file_info->data[file_path].last_write_time));
			//out->logs << OUTDEBUG << file_path << ", attribute.last_write_time_s=" << file_info->data[file_path].last_write_time_s;
			file_info->data[file_path].user = get_file_uid(file_path);
			//out->logs << OUTDEBUG << file_path << ", attribute.user=" << file_info->data[file_path].user;
			file_info->data[file_path].user_name = user_group->get_user_name(file_info->data[file_path].user);
			//out->logs << OUTDEBUG << file_path << ", attribute.user_name=" << file_info->data[file_path].user_name;
			file_info->data[file_path].group = get_file_gid(file_path);
			//out->logs << OUTDEBUG << file_path << ", attribute.group=" << file_info->data[file_path].group;
			file_info->data[file_path].group_name = user_group->get_group_name(file_info->data[file_path].group);
			//out->logs << OUTDEBUG << file_path << ", attribute.group_name=" << file_info->data[file_path].group_name;
			file_info->data[file_path].status = true;
			//out->logs << OUTDEBUG << file_path << ", attribute.status=" << file_info->data[file_path].status;
		}
		catch (boost::filesystem::filesystem_error& e)
		{
			file_info->data[file_path].status = false;
			out->logs << OUTERROR << e.what();
		}
		return file_info->data[file_path];
	}
	int file_system_operation::get_file_uid(const string& file_path) //获取文件的uid
	{
		struct stat st;
		try
		{
			if (stat(file_path.c_str(), &st) == -1)
			{
				throw exception() << err_str(file_path + strerror(errno));
			}
			return (int)st.st_uid;
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return -1;
		}
	}
	int file_system_operation::get_file_gid(const string& file_path) //获取文件的gid
	{
		struct stat st;
		try
		{
			if (stat(file_path.c_str(), &st) == -1)
			{
				throw exception() << err_str(file_path + strerror(errno));
			}
			return (int)st.st_gid;
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return -1;
		}
	}

	void file_system_operation::show_file_info(const string& file_path) //显示文件的属性
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			get_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			out->logs << OUTDEBUG << file_path << ", attribute.file_path=" << file_info->data[file_path].file_path;
			out->logs << OUTDEBUG << file_path << ", attribute.file_size=" << file_info->data[file_path].file_size;
			out->logs << OUTDEBUG << file_path << ", attribute.type=" << file_info->data[file_path].type;
			out->logs << OUTDEBUG << file_path << ", attribute.type_name=" << file_info->data[file_path].type_name;
			out->logs << OUTDEBUG << file_path << ", attribute.permissions=" << file_info->data[file_path].permissions;
			out->logs << OUTDEBUG << file_path << ", attribute.permissions_name=" << file_info->data[file_path].permissions_name;
			out->logs << OUTDEBUG << file_path << ", attribute.last_write_time=" << file_info->data[file_path].last_write_time;
			out->logs << OUTDEBUG << file_path << ", attribute.last_write_time_s=" << file_info->data[file_path].last_write_time_s;
			out->logs << OUTDEBUG << file_path << ", attribute.user=" << file_info->data[file_path].user;
			out->logs << OUTDEBUG << file_path << ", attribute.user_name=" << file_info->data[file_path].user_name;
			out->logs << OUTDEBUG << file_path << ", attribute.group=" << file_info->data[file_path].group;
			out->logs << OUTDEBUG << file_path << ", attribute.group_name=" << file_info->data[file_path].group_name;
			out->logs << OUTDEBUG << file_path << ", attribute.status=" << file_info->data[file_path].status;
		}
	}
}