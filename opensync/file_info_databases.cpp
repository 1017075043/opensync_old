#include "file_info_databases.h"

namespace opensync
{
	file_info_databases* file_info_databases::instance = nullptr;

	file_info_databases::file_info_databases()
	{
		init_file_type_dictionary();
		init_file_permissions_dictionary();
	}
	file_info_databases::~file_info_databases()
	{

	}
	file_info_databases* file_info_databases::get_instance()
	{
		return instance;
	}
	file_info_databases* file_info_databases::init_instance()
	{
		if (instance == nullptr) {
			instance = new file_info_databases();
		}
		return instance;
	}
	void file_info_databases::destory()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}
	void file_info_databases::init_file_type_dictionary() //初始化文件类型字典
	{
		file_type_dictionary.left.insert(make_pair(0, "status_error"));
		file_type_dictionary.left.insert(make_pair(1, "file_not_found"));
		file_type_dictionary.left.insert(make_pair(2, "regular_file"));
		file_type_dictionary.left.insert(make_pair(3, "directory_file"));
		file_type_dictionary.left.insert(make_pair(4, "symlink_file"));
		file_type_dictionary.left.insert(make_pair(5, "block_file"));
		file_type_dictionary.left.insert(make_pair(6, "character_file"));
		file_type_dictionary.left.insert(make_pair(7, "fifo_file"));
		file_type_dictionary.left.insert(make_pair(8, "socket_file"));
		file_type_dictionary.left.insert(make_pair(9, "reparse_file"));
		file_type_dictionary.left.insert(make_pair(10, "type_unknown"));
		file_type_dictionary.left.insert(make_pair(11, "_detail_directory_symlink"));
	}
	string file_info_databases::transfrom_file_type(int type)
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
	int file_info_databases::transfrom_file_type(string type)
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
	void file_info_databases::init_file_permissions_dictionary() //初始化文件权限字典
	{
		boost::format fmt("%03d");
		for (int user = 0; user <= 7; ++user)
		{
			for (int group = 0; group <= 7; ++group)
			{
				for (int other = 0; other <= 7; ++other)
				{
					file_permissions_dictionary.left.insert(make_pair(user * 8 * 8 + group * 8 + other, (fmt % to_string(user * 100 + group * 10 + other)).str()));
					//out->logs << OUTDEBUG << user * 8 * 8 + group * 8 + other << " = " << (fmt % to_string(user * 100 + group * 10 + other)).str();
				}
			}
		}
	}
	string file_info_databases::transfrom_file_permissions(int permissions) //转换文件权限
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
	int file_info_databases::transfrom_file_permissions(string permissions) //转换文件权限
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

/*
enum file_type
{
	status_error,				//0
	file_not_found,				//1
	regular_file,				//2
	directory_file,				//3
	symlink_file,				//4
	block_file,					//5
	character_file,				//6
	fifo_file,					//7
	socket_file,				//8
	reparse_file,				//9
	type_unknown,				//10
	_detail_directory_symlink	//11
};
enum perms
{
	no_perms = 0,       // file_not_found is no_perms rather than perms_not_known
	owner_read = 0400,  // S_IRUSR, Read permission, owner
	owner_write = 0200, // S_IWUSR, Write permission, owner
	owner_exe = 0100,   // S_IXUSR, Execute/search permission, owner
	owner_all = 0700,   // S_IRWXU, Read, write, execute/search by owner
	group_read = 040,   // S_IRGRP, Read permission, group
	group_write = 020,  // S_IWGRP, Write permission, group
	group_exe = 010,    // S_IXGRP, Execute/search permission, group
	group_all = 070,    // S_IRWXG, Read, write, execute/search by group
	others_read = 04,   // S_IROTH, Read permission, others
	others_write = 02,  // S_IWOTH, Write permission, others
	others_exe = 01,    // S_IXOTH, Execute/search permission, others
	others_all = 07,    // S_IRWXO, Read, write, execute/search by others
	all_all = 0777,     // owner_all|group_all|others_all
	set_uid_on_exe = 04000, // S_ISUID, Set-user-ID on execution
	set_gid_on_exe = 02000, // S_ISGID, Set-group-ID on execution
	sticky_bit = 01000, // S_ISVTX,
	perms_mask = 07777,     // all_all|set_uid_on_exe|set_gid_on_exe|sticky_bit
	perms_not_known = 0xFFFF, // present when directory_entry cache not loaded
	add_perms = 0x1000,     // adds the given permission bits to the current bits
	remove_perms = 0x2000,  // removes the given permission bits from the current bits;
	symlink_perms = 0x4000, // on POSIX, don't resolve symlinks; implied on Windows
	_detail_extend_perms_32_1 = 0x7fffffff,
	_detail_extend_perms_32_2 = -0x7fffffff - 1
};
//*/