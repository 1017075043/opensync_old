#include "file_info_databases.h"

namespace opensync
{
	bool file_info_databases::is_exist_file_type(const int& type) //判断转换文件类型是否存在
	{
		if (file_type_dictionary.left.find(type) != file_type_dictionary.left.end())
		{
			return true;
		}
		out->logs << OUTWARN << type << " is not exist";
		return false;
	}
	bool file_info_databases::is_exist_file_type(const string& type) //判断转换文件类型是否存在
	{
		if (file_type_dictionary.right.find(type) != file_type_dictionary.right.end())
		{
			return true;
		}
		out->logs << OUTWARN << type << " is not exist";
		return false;
	}
	bool file_info_databases::is_exist_file_permissions(const int& permissions) //判断文件权限是否存在
	{
		if (file_permissions_dictionary.left.find(permissions) != file_permissions_dictionary.left.end())
		{
			return true;
		}
		out->logs << OUTWARN << permissions << " is not exist";
		return false;
	}
	bool file_info_databases::is_exist_file_permissions(const string& permissions) //判断文件权限是否存在
	{
		if (file_permissions_dictionary.right.find(permissions) != file_permissions_dictionary.right.end())
		{
			return true;
		}
		out->logs << OUTWARN << permissions << " is not exist";
		return false;
	}
}