#pragma once

#include "opensync_public_include_file.h"

#include <limits.h>
#include <sys/epoll.h>
#include <sys/fanotify.h>

#include "system_action.h"

/*
	文件系统监控类fanotify(单例)
	@author 吴南辉
	@time 2020/06/25
*/

namespace opensync
{
	class file_system_fanotify : boost::noncopyable
	{
	private:
		static file_system_fanotify* instance;
		opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();
		system_action sys_ac;

		int fanotify_fd; //保存fanotify_init返回的文件描述符

	private:
		//file_system_fanotify
		file_system_fanotify();
		~file_system_fanotify();

	public:
		//file_system_fanotify
		static file_system_fanotify* get_instance();
		static file_system_fanotify* init_instance();
		static void destory();

	public:
		bool create_fanotify(int& fd); //创建监听fanotify实例
		bool add_watch_unit(const string& file_path);//向fanotify中添加一个监控（只监控目录）
		bool start_watch(); //开始监听
		bool handle_event(const int& event_count); //处理事件
		bool handle_event_son1(const struct fanotify_event_metadata* metadata, const string& path); //处理事件
	};
}


