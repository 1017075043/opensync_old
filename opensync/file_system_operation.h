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
	�ļ�ϵͳ������
	@author ���ϻ�
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
		int get_file_uid(const string& file_path); //��ȡ�ļ���uid
		int get_file_gid(const string& file_path); //��ȡ�ļ���gid
		string get_file_md5_p(const string& file_path); //��ȡ�ļ�md5ֵ
	public:
		file_system_operation();
		~file_system_operation();
		opensync::file_attribute get_file_info(const string& file_path); //��ȡһ���ļ�������Ϣ
		void show_file_info(const string& file_path); //��ʾ�ļ�������
	};
}

