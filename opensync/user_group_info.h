#pragma once
#include "opensync_public_include_file.h"

#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

#include <boost/bimap.hpp>
#include <boost/noncopyable.hpp>

using namespace std;

/*
	ϵͳ�û����û����ࣨ������
	@author ���ϻ�
	@time 2020/05/30
*/

namespace opensync
{
	class user_group_info : boost::noncopyable
	{
	private:
		string passwd_file = "/etc/passwd";
		string group_file = "/etc/group";

		typedef boost::bimap<int, string> bimap;
		bimap user; //����û���Ϣ
		bimap group; //����û�����Ϣ

		static user_group_info* instance;
		opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();
	private:
		//user_group_info
		user_group_info();
		~user_group_info();
	public:
		//user_group_info
		static user_group_info* get_instance();
		static user_group_info* init_instance();
		static void destory();

	public:
		//user_group_info
		bool init_system_all_user_group_info(); //��ȡϵͳ�����û����û�����Ϣ
		bool init_system_all_user_info(); //��ȡϵͳ�����û���Ϣ
		bool init_system_all_group_info(); //��ȡϵͳ�������û���Ϣ

		//user_group_info_get
		string get_user_name(const int& id); //��ȡ�û���
		int get_user_id(const string& name); //��ȡ�û�ID
		string get_group_name(const int& id); //��ȡ�û�����
		int get_group_id(const string& name); //��ȡ�û���ID
		
		//user_group_info_is_exist
		bool is_exist_user_name(const string& name); //�ж��Ƿ�����û���
		bool is_exist_user_id(const int& id); //�ж��Ƿ�����û�ID
		bool is_exist_group_name(const string& name); //�ж��Ƿ�����û�����
		bool is_exist_group_id(const int& id); //�ж��Ƿ�����û���ID

		//user_group_info_show
		void show_all_user_info(); //��ʾ�����û���Ϣ
		void show_all_group_info(); //��ʾ�����û�����Ϣ
	};
}

