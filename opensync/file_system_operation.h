#pragma once

#include "opensync_public_include_file.h"

#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/des.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>


#include "file_info_databases.h"
#include "user_group_info.h"

/*
	文件系统操作类
	@author 吴南辉
	@time 2020/05/30
*/

namespace opensync
{
	class file_system_operation
	{
	private:
		opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();
		opensync::user_group_info* user_group = opensync::user_group_info::init_instance();
		opensync::file_info_databases* file_info = opensync::file_info_databases::init_instance();
	private:
		int get_file_uid(const string& file_path); //获取文件的uid
		int get_file_gid(const string& file_path); //获取文件的gid
		string get_file_md5_p(const string& file_path); //获取文件md5值
	public:
		file_system_operation();
		~file_system_operation();
		opensync::file_attribute get_file_info(const string& file_path); //获取一个文件属性信息
		void show_file_info(const string& file_path); //显示文件的属性
	};
}

