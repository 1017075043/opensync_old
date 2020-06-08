#include "file_system_operation.h"
namespace opensync
{
	void file_system_operation::show_file_info_p(const string& file_path) //显示文件的属性（私用）
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
		out->logs << OUTDEBUG << file_path << ", attribute.file_hash=" << file_info->data[file_path].file_hash;
		out->logs << OUTDEBUG << file_path << ", attribute.status=" << file_info->data[file_path].status;
	}
	void file_system_operation::show_file_info(const string& file_path) //显示文件的属性
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			show_file_info_p(file_path);
		}
	}
	void file_system_operation::show_file_info_databases() //显示目前文件信息库中存储文件的信息
	{
		map<string, file_attribute>::iterator it;
		for (it = file_info->data.begin(); it != file_info->data.end(); it++)
		{
			show_file_info_p(it->first);
		}
	}
}