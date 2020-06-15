#pragma once

#include "opensync_public_include_file.h"

#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem/fstream.hpp>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/des.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>


#include "file_info_databases.h"
#include "user_group_info.h"
#include "string_operation.h"

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
		//file_system_operation
		void init_file_info(const string& file_path); //��ʼ���ļ���Ϣ

		//file_system_operation_get
		int get_file_uid(const string& file_path); //��ȡ�ļ���uid
		int get_file_gid(const string& file_path); //��ȡ�ļ���gid
		string get_file_md5_p(const string& file_path); //��ȡ�ļ�md5ֵ

		//file_system_operation_show
		void show_file_info_p(const string& file_path); //��ʾ�ļ������ԣ�˽�ã�
	public:
		//file_system_operation
		file_system_operation();
		~file_system_operation();

		//file_system_operation_get
		const opensync::file_attribute* get_file_info(const string& file_path); //��ȡһ���ļ�������Ϣ
		const vector<const opensync::file_attribute*> get_file_and_dir_traverse_all_list(const string& file_path); //������ȡһ��Ŀ¼��Ŀ¼�����¼����������ļ���������Ϣ�б�
		const vector<const opensync::file_attribute*> get_file_and_dir_traverse_dir_list(const string& file_path); //������ȡһ��Ŀ¼��Ŀ¼�����¼�Ŀ¼�����ļ���������Ϣ�б�
		const vector<const opensync::file_attribute*> get_file_and_dir_traverse_file_list(const string& file_path);//������ȡһ��Ŀ¼��Ŀ¼�����¼���Ŀ¼�����ļ���������Ϣ�б�
		const path get_file_path(const string& file_path); //��ȡ�ļ�file_path
		const long get_file_size(const string& file_path); //��ȡ�ļ�file_size
		const file_type get_file_type(const string& file_path); //��ȡ�ļ�file_type
		const string get_file_type_name(const string& file_path); //��ȡ�ļ�file_type_name
		const perms get_file_permissions(const string& file_path); //��ȡ�ļ�file_permissions
		const string get_file_permissions_name(const string& file_path); //��ȡ�ļ�file_permissions_name
		const time_t get_file_last_write_time(const string& file_path); //��ȡ�ļ�file_last_write_time
		const string get_file_last_write_time_s(const string& file_path); //��ȡ�ļ�file_last_write_time_s
		const int get_file_user(const string& file_path); //��ȡ�ļ�file_user
		const string get_file_user_name(const string& file_path); //��ȡ�ļ�file_user_name
		const int get_file_group(const string& file_path); //��ȡ�ļ�file_group
		const string get_file_group_name(const string& file_path); //��ȡ�ļ�file_group_name
		const string get_file_hash(const string& file_path); //��ȡ�ļ�file_hash
		const bool get_file_status(const string& file_path); //��ȡ�ļ�file_status

		//file_system_operation_show
		void show_file_info(const string& file_path); //��ʾ�ļ�������
		void show_file_info_databases(); //��ʾĿǰ�ļ���Ϣ���д洢�ļ�����Ϣ

		//file_system_operation_update
		void update_file_info(const string& file_path); //�����ļ���Ϣ
		bool update_file_size(const string& file_path, const long& file_size); //�����ļ�file_size
		bool update_file_type(const string& file_path, const file_type& type); //�����ļ�file_type
		bool update_file_type_name(const string& file_path, const string& type_name); //�����ļ�file_type_name
		bool update_file_permissions(const string& file_path, const perms& permissions); //�����ļ�file_permissions
		bool update_file_permissions_name(const string& file_path, const string& permissions_name); //�����ļ�file_permissions_name
		bool update_file_last_write_time(const string& file_path, const time_t& last_write_time); //�����ļ�file_last_write_time
		bool update_file_last_write_time_s(const string& file_path, const string& last_write_time_s); //�����ļ�file_last_write_time_s
		bool update_file_user(const string& file_path, const int& user); //�����ļ�file_user
		bool update_file_user_name(const string& file_path, const string& user_name); //�����ļ�file_user_name
		bool update_file_group(const string& file_path, const int& group); //�����ļ�file_group
		bool update_file_group_name(const string& file_path, const string& group_name); //�����ļ�file_group_name
		bool update_file_hash(const string& file_path, const string& file_hash); //�����ļ�file_hash
		bool update_file_status(const string& file_path, const bool& status); //�����ļ�file_status

		//file_system_operation_set
		bool set_file_permissions(const string& file_path, const perms& permissions);//�����ļ�file_permissions
		bool set_file_permissions(const string& file_path, const string& permissions_name);//�����ļ�file_permissions_name
		bool set_file_last_write_time(const string& file_path, const time_t& last_write_time); //�����ļ�file_last_write_time
		bool set_file_last_write_time_s(const string& file_path, const string& last_write_time_s); //�����ļ�file_last_write_time_s
		bool set_file_user(const string& file_path, const int& user);//�����ļ�file_user
		bool set_file_user(const string& file_path, const string& user_name);//�����ļ�file_user_name
		bool set_file_group(const string& file_path, const int& group);//�����ļ�file_user
		bool set_file_group(const string& file_path, const string& group_name);//�����ļ�file_group_name
		bool set_file_user_group(const string& file_path, const int& user, const int& group);//�����ļ�file_user��file_user_name
		bool set_file_user_group(const string& file_path, const string& user_name, const string& group_name);//�����ļ�file_user_name��file_user_name
	};
}

