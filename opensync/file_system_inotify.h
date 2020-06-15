#pragma once

#include "opensync_public_include_file.h"
#include <sys/epoll.h>
#include <list>
#include <sys/inotify.h>
#include <boost/bimap.hpp>

#include "file_system_operation.h"

/*
	文件系统监控类(单例)
	@author 吴南辉
	@time 2020/06/13
*/

namespace opensync
{
	class file_system_inotify : boost::noncopyable
	{
	private:
		static file_system_inotify* instance;
		opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();
		opensync::file_system_operation file_op;

		int inotify_fd; //保存inotify_init返回的文件描述符
		typedef boost::bimap<int, string> bimap;
		bimap inotify_list; //监控列表
		list<string> watch_list; //监听文件列表
		list<string> ignore_watch_list; //忽略监听文件列表

	private:
		//file_system_inotify
		file_system_inotify();
		~file_system_inotify();
		bool create_inotify(int& fd); //创建监听inotify实例
		bool add_watch_unit(const string& file_path);//向inotify中添加一个监控（只监控目录）
		bool add_watch_dir(const string& dir_path);//向inotify中添加一个目录监控,会监控目录下（包括子目录）的所有动作（只监控目录）
		bool del_watch_unit(const int& wd, const string& file_path);//向inotify中去除一个监控

	public:
		//file_system_inotify
		static file_system_inotify* get_instance();
		static file_system_inotify* init_instance();
		static void destory();

	public:
		//file_system_inotify
		bool open_inotify(); //开始进行监控
		void handle_event(const int& wd, const string& name, const uint32_t& mask); //处理一个事件
		inline const string transform_event_mask(const uint32_t& mask); //事件转换
		inline const string transform_event_mask_chinese(const uint32_t& mask); //事件转换(中文)
		//file_system_inotify
		

		//file_system_inotify_list_operation
		bool is_exists_inotifyed_file(const string& file_path); //判断一个文件是否处于监听（只监控目录）
		string get_inotifyed_file(const int& wd); //根据wd从监控列表中获取文件名
		int get_inotifyed_file(const string& file_path); //根据文件路径从监控列表中获取wd
		void add_watch(const string& file_path); //添加监听
		void add_ignore_watch(const string& file_path); //添加忽略监听

	};
}

