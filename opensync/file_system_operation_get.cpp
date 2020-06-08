#include "file_system_operation.h"
namespace opensync
{
	const opensync::file_attribute* file_system_operation::get_file_info(const string& file_path) //获取一个文件属性信息
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		return &file_info->data[file_path];
	}
	int file_system_operation::get_file_uid(const string& file_path) //获取文件的uid
	{
		struct stat st;
		try
		{
			if (stat(file_path.c_str(), &st) == -1)
			{
				throw exception() << err_str(file_path + " " + strerror(errno));
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
				throw exception() << err_str(file_path + " " + strerror(errno));
			}
			return (int)st.st_gid;
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return -1;
		}
	}
	string file_system_operation::get_file_md5_p(const string& file_path) //获取文件md5值
	{
		string md5_value;
		try
		{
			std::ifstream file(file_path.c_str(), std::ifstream::binary);
			if (!file)
			{
				throw exception() << err_str(file_path + " " + strerror(errno));
			}
			MD5_CTX md5Context;
			MD5_Init(&md5Context);

			char buf[1024 * 16];
			while (file.good())
			{
				file.read(buf, sizeof(buf));
				MD5_Update(&md5Context, buf, file.gcount());
			}

			unsigned char result[MD5_DIGEST_LENGTH];
			MD5_Final(result, &md5Context);

			char hex[35];
			memset(hex, 0, sizeof(hex));
			for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
			{
				sprintf(hex + i * 2, "%02x", result[i]);
			}
			hex[32] = '\0';
			md5_value = string(hex);
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
		}
		return md5_value;
	}
	const vector<const opensync::file_attribute*> file_system_operation::get_file_and_son_info(const string& file_path) //获取一个文件及其下级文件的属性信息
	{
		typedef boost::filesystem::recursive_directory_iterator rd_iterator;
		vector<const opensync::file_attribute*> file_and_son_info;
		try
		{
			boost::filesystem::path p(file_path);
			if (!exists(p))
			{
				throw exception() << err_str(file_path + " " + strerror(errno));
			}
			if (file_info->data.find(file_path) == file_info->data.end())
			{
				init_file_info(file_path);
			}
			file_and_son_info.push_back(&file_info->data[file_path]);
			if (boost::filesystem::is_directory(p))
			{
				rd_iterator end;
				for (rd_iterator pos(p); pos != end; ++pos)
				{
					get_file_info((const string&)pos->path());
					file_and_son_info.push_back(&file_info->data[(const string&)pos->path()]);
				}
			}
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
		}
		return file_and_son_info;
	}
	const path file_system_operation::get_file_path(const string& file_path) //获取文件file_path
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].file_path;
		}
		return (path)"";
	}
	const long file_system_operation::get_file_size(const string& file_path) //获取文件file_size
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].file_size;
		}
		return (long)-1;
	}
	const file_type file_system_operation::get_file_type(const string& file_path) //获取文件file_type
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].type;
		}
		return (file_type)0;
	}
	const string file_system_operation::get_file_type_name(const string& file_path) //获取文件file_type_name
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].type_name;
		}
		return "";
	}
	const perms file_system_operation::get_file_permissions(const string& file_path) //获取文件file_permissions
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].permissions;
		}
		return (perms)-1;
	}
	const string file_system_operation::get_file_permissions_name(const string& file_path) //获取文件file_permissions_name
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].permissions_name;
		}
		return "";
	}
	const time_t file_system_operation::get_file_last_write_time(const string& file_path) //获取文件file_last_write_time
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].last_write_time;
		}
		return (time_t)-1;
	}
	const string file_system_operation::get_file_last_write_time_s(const string& file_path) //获取文件file_last_write_time_s
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].last_write_time_s;
		}
		return "";
	}
	const int file_system_operation::get_file_user(const string& file_path) //获取文件file_user
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].user;
		}
		return (int)-1;
	}
	const string file_system_operation::get_file_user_name(const string& file_path) //获取文件file_user_name
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].user_name;
		}
		return "";
	}
	const int file_system_operation::get_file_group(const string& file_path) //获取文件file_group
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].group;
		}
		return (int)-1;
	}
	const string file_system_operation::get_file_group_name(const string& file_path) //获取文件file_group_name
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].group_name;
		}
		return "";
	}
	const string file_system_operation::get_file_hash(const string& file_path) //获取文件file_hash
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		if (file_info->data[file_path].status)
		{
			return file_info->data[file_path].file_hash;
		}
		return "";
	}
	const bool file_system_operation::get_file_status(const string& file_path) //获取文件file_status
	{
		if (file_info->data.find(file_path) == file_info->data.end())
		{
			init_file_info(file_path);
		}
		return file_info->data[file_path].status;
	}
}