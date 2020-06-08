#include "file_info_databases.h"

namespace opensync
{
	string file_info_databases::transfrom_file_type(const int& type)
	{
		bimap::left_const_iterator it;
		it = file_type_dictionary.left.find(type);
		if (it != file_type_dictionary.left.end())
		{
			//out->logs << OUTDEBUG << type << ":" << it->second;
			return it->second;
		}
		out->logs << OUTWARN << type << " is not exist";
		return "";
	}
	int file_info_databases::transfrom_file_type(const string& type)
	{
		bimap::right_const_iterator it;
		it = file_type_dictionary.right.find(type);
		if (it != file_type_dictionary.right.end())
		{
			//out->logs << OUTDEBUG << type << ":" << it->second;
			return it->second;
		}
		out->logs << OUTWARN << type << " is not exist";
		return -1;
	}
	string file_info_databases::transfrom_file_permissions(const int& permissions) //转换文件权限
	{
		bimap::left_const_iterator it;
		it = file_permissions_dictionary.left.find(permissions);
		if (it != file_permissions_dictionary.left.end())
		{
			//out->logs << OUTDEBUG << permissions << ":" << it->second;
			return it->second;
		}
		out->logs << OUTWARN << permissions << " is not exist";
		return "";
	}
	int file_info_databases::transfrom_file_permissions(const string& permissions) //转换文件权限
	{
		bimap::right_const_iterator it;
		it = file_permissions_dictionary.right.find(permissions);
		if (it != file_permissions_dictionary.right.end())
		{
			//out->logs << OUTDEBUG << permissions << ":" << it->second;
			return it->second;
		}
		out->logs << OUTWARN << permissions << " is not exist";
		return -1;
	}
}