#include "file_system_operation.h"
namespace opensync
{
	file_system_operation::file_system_operation()
	{
	}
	file_system_operation::~file_system_operation()
	{
	}
	void file_system_operation::init_file_info(const string& file_path) //初始化文件信息
	{
		try
		{
			out->logs << OUTINFO << "create file info:" << file_path;
			boost::filesystem::path p(file_path);
			boost::filesystem::file_status s = boost::filesystem::status(p);

			file_info->data[file_path].file_path = p;
			//out->logs << OUTDEBUG << file_path << ", attribute.file_path=" << file_info->data[file_path].file_path;
			if (!boost::filesystem::is_directory(p))
			{
				file_info->data[file_path].file_size = (long)boost::filesystem::file_size(p);
				//out->logs << OUTDEBUG << file_path << ", attribute.file_size=" << file_info->data[file_path].file_size;
				file_info->data[file_path].file_hash = get_file_md5_p(file_path);
				//out->logs << OUTDEBUG << file_path << ", attribute.file_hash=" << file_info->data[file_path].file_hash;
			}
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
		//return file_info->data[file_path];
	}

}