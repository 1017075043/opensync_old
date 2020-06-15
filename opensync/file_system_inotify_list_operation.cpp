#include "file_system_inotify.h"
namespace opensync
{
    bool file_system_inotify::is_exists_inotifyed_file(const string& file_path) //判断一个文件是否处于监听
    {
		bimap::right_const_iterator it;
		it = inotify_list.right.find(file_path);
		if (it != inotify_list.right.end())
		{
			return true;
		}
		return false;
    }
	string file_system_inotify::get_inotifyed_file(const int& wd) //根据wd从监控列表中获取文件名
	{
		bimap::left_const_iterator it;
		it = inotify_list.left.find(wd);
		if (it != inotify_list.left.end())
		{
			return it->second;
		}
		return "";
	}
	int file_system_inotify::get_inotifyed_file(const string& file_path) //根据文件路径从监控列表中获取wd
	{
		bimap::right_const_iterator it;
		it = inotify_list.right.find(file_path);
		if (it != inotify_list.right.end())
		{
			return it->second;
		}
		return -1;
	}
	void file_system_inotify::add_watch(const string& file_path) //添加监听
	{
		watch_list.push_back(file_path);
	}
	void file_system_inotify::add_ignore_watch(const string& file_path) //添加忽略监听
	{
		ignore_watch_list.push_back(file_path);
	}

}