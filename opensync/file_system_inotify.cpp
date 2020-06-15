#include "file_system_inotify.h"
namespace opensync
{
	file_system_inotify* file_system_inotify::instance = nullptr;
	file_system_inotify::file_system_inotify()
	{
		create_inotify(inotify_fd);
	}
	file_system_inotify::~file_system_inotify()
	{
	}
	file_system_inotify* file_system_inotify::get_instance()
	{
		return instance;
	}
	file_system_inotify* file_system_inotify::init_instance()
	{
		if (instance == nullptr) {
			instance = new file_system_inotify();
		}
		return instance;
	}
	void file_system_inotify::destory()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}
	inline const string file_system_inotify::transform_event_mask(const uint32_t& mask) //事件转换
	{
		string event_list;
		if (mask & IN_ACCESS) event_list = event_list + "IN_ACCESS|"; /* 文件被访问.  */
		if (mask & IN_MODIFY) event_list = event_list + "IN_MODIFY|"; /* 文件被修改.  */
		if (mask & IN_ATTRIB) event_list = event_list + "IN_ATTRIB|"; /* 元数据发生了变化.  */
		if (mask & IN_CLOSE_WRITE) event_list = event_list + "IN_CLOSE_WRITE|"; /* 可写入文件关闭.  */
		if (mask & IN_CLOSE_NOWRITE) event_list = event_list + "IN_CLOSE_NOWRITE|"; /* 不可写文件关闭.  */
		if (mask & IN_CLOSE_NOWRITE && mask & IN_CLOSE_NOWRITE) event_list = event_list + "IN_CLOSE|"; /* 文件已关闭.  */
		if (mask & IN_OPEN) event_list = event_list + "IN_OPEN|"; /* 文件被打开.  */
		if (mask & IN_MOVED_FROM) event_list = event_list + "IN_MOVED_FROM|"; /* 文件从X移动.  */
		if (mask & IN_MOVED_TO) event_list = event_list + "IN_MOVED_TO|"; /* 文件被移动到Y.  */
		if (mask & IN_MOVED_FROM && mask & IN_MOVED_TO) event_list = event_list + "IN_MOVE|"; /* 文件被移动.  */
		if (mask & IN_CREATE) event_list = event_list + "IN_CREATE|"; /* 子文件创建.  */
		if (mask & IN_DELETE) event_list = event_list + "IN_DELETE|"; /* 子文件被删除.  */
		if (mask & IN_DELETE_SELF) event_list = event_list + "IN_DELETE_SELF|"; /* 自我被删除.  */
		if (mask & IN_MOVE_SELF) event_list = event_list + "IN_MOVE_SELF|"; /* 自我被移动.  */
		if (mask & IN_UNMOUNT) event_list = event_list + "IN_UNMOUNT|"; /* 已卸载备份fs.  */
		if (mask & IN_Q_OVERFLOW) event_list = event_list + "IN_Q_OVERFLOW|"; /* 事件队列溢出.  */
		if (mask & IN_IGNORED) event_list = event_list + "IN_IGNORED|"; /* 文件被忽视.  */
		if (mask & IN_ONLYDIR) event_list = event_list + "IN_ONLYDIR|"; /* 只有在路径是目录的情况下才查看路径.  */
		if (mask & IN_DONT_FOLLOW) event_list = event_list + "IN_DONT_FOLLOW|"; /* 不遵循一个sym链接.  */
		if (mask & IN_EXCL_UNLINK) event_list = event_list + "IN_EXCL_UNLINK|"; /* 排除未链接对象上的事件sym链接.  */
		if (mask & IN_MASK_ADD) event_list = event_list + "IN_MASK_ADD|"; /* 添加到一个已经存在的监视的掩码.  */
		if (mask & IN_ISDIR) event_list = event_list + "IN_ISDIR|"; /* 针对dir发生的事件.  */
		if (mask & IN_ONESHOT) event_list = event_list + "IN_ONESHOT|"; /* 只发送事件一次.  */
		return event_list;
	}
	inline const string file_system_inotify::transform_event_mask_chinese(const uint32_t& mask) //事件转换(中文)
	{
		string event_list;
		if (mask & IN_ACCESS) event_list = event_list + "文件被访问|"; /* 文件被访问.  */
		if (mask & IN_MODIFY) event_list = event_list + "文件被修改|"; /* 文件被修改.  */
		if (mask & IN_ATTRIB) event_list = event_list + "元数据发生了变化|"; /* 元数据发生了变化.  */
		if (mask & IN_CLOSE_WRITE) event_list = event_list + "可写入文件关闭|"; /* 可写入文件关闭.  */
		if (mask & IN_CLOSE_NOWRITE) event_list = event_list + "不可写文件关闭|"; /* 不可写文件关闭.  */
		if (mask & IN_CLOSE_NOWRITE && mask & IN_CLOSE_NOWRITE) event_list = event_list + "文件已关闭|"; /* 文件已关闭.  */
		if (mask & IN_OPEN) event_list = event_list + "文件被打开|"; /* 文件被打开.  */
		if (mask & IN_MOVED_FROM) event_list = event_list + "文件从X移动|"; /* 文件从X移动.  */
		if (mask & IN_MOVED_TO) event_list = event_list + "文件被移动到Y|"; /* 文件被移动到Y.  */
		if (mask & IN_MOVED_FROM && mask & IN_MOVED_TO) event_list = event_list + "文件被移动|"; /* 文件被移动.  */
		if (mask & IN_CREATE) event_list = event_list + "子文件创建|"; /* 子文件创建.  */
		if (mask & IN_DELETE) event_list = event_list + "子文件被删除|"; /* 子文件被删除.  */
		if (mask & IN_DELETE_SELF) event_list = event_list + "自我被删除|"; /* 自我被删除.  */
		if (mask & IN_MOVE_SELF) event_list = event_list + "自我被移动|"; /* 自我被移动.  */
		if (mask & IN_UNMOUNT) event_list = event_list + "已卸载备份fs|"; /* 已卸载备份fs.  */
		if (mask & IN_Q_OVERFLOW) event_list = event_list + "事件队列溢出|"; /* 事件队列溢出.  */
		if (mask & IN_IGNORED) event_list = event_list + "文件被忽视|"; /* 文件被忽视.  */
		if (mask & IN_ONLYDIR) event_list = event_list + "只有在路径是目录的情况下才查看路径|"; /* 只有在路径是目录的情况下才查看路径.  */
		if (mask & IN_DONT_FOLLOW) event_list = event_list + "不遵循一个sym链接|"; /* 不遵循一个sym链接.  */
		if (mask & IN_EXCL_UNLINK) event_list = event_list + "排除未链接对象上的事件sym链接|"; /* 排除未链接对象上的事件sym链接.  */
		if (mask & IN_MASK_ADD) event_list = event_list + "添加到一个已经存在的监视的掩码|"; /* 添加到一个已经存在的监视的掩码.  */
		if (mask & IN_ISDIR) event_list = event_list + "针对dir发生的事件|"; /* 针对dir发生的事件.  */
		if (mask & IN_ONESHOT) event_list = event_list + "只发送事件一次|"; /* 只发送事件一次.  */
		return event_list;
	}

	bool file_system_inotify::create_inotify(int& fd)  //创建监听inotify实例
	{
		fd = inotify_init();
		try
		{
			if (fd < 0)
			{
				throw exception() << err_str("cteate inotify fail, fd:" + to_string(fd) + ", mesg=" + strerror(errno));
			}
			else
			{
				out->logs << OUTINFO << "cteate inotify success, fd:" << fd;
			}
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return false;
		}
		return true;
	}

	bool file_system_inotify::add_watch_unit(const string& file_path) //向inotify中添加一个监控（只监控目录）
	{
		BOOST_FOREACH(string ignore_file_path, ignore_watch_list) //检查是否忽略监听
		{
			if (ignore_file_path == file_path.substr(0, ignore_file_path.length()))
			{
				out->logs << OUTDEBUG << "ignore watch: " << file_path;
				return true;
			}
		}
		if (file_op.get_file_status(file_path) == false) //如果文件不存在
		{
			return false;
		}
		string dir_path = file_path;
		if (file_op.get_file_type(dir_path) != 3) //如果文件不是目录
		{
			dir_path = file_op.get_file_path(dir_path).parent_path().string();
			out->logs << OUTDEBUG << file_path << " is file, get it parent_path=" << dir_path;
		}
		if (is_exists_inotifyed_file(file_path)) //是否已经处于监听状态
		{
			out->logs << OUTINFO << file_path << " is inotifyed status";
			return true;
		}
		try
		{
			int wd = inotify_add_watch(inotify_fd, file_path.c_str(), IN_ALL_EVENTS); //添加监控
			if (wd < 0)
			{
				throw exception() << err_str(file_path + " add inotifyed fail, mesg=" + strerror(errno));
			}
			else
			{
				inotify_list.left.insert(make_pair(wd, file_path)); //加入监控列表
				out->logs << OUTINFO << file_path << " add inotifyed success, wd=" << wd;
			}
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return false;
		}
		return true;
	}
	bool file_system_inotify::add_watch_dir(const string& dir_path)//向inotify中添加一个目录监控,会监控目录下（包括子目录）的所有动作（只监控目录）
	{
		vector<const opensync::file_attribute*> file_attr_list;
		file_attr_list = file_op.get_file_and_dir_traverse_dir_list(dir_path);
		BOOST_FOREACH(const opensync::file_attribute * file_attr, file_attr_list)
		{
			if (add_watch_unit(file_attr->file_path.string()) == false)
			{
				return false;
			}
		}
		return true;
	}

	bool file_system_inotify::open_inotify() //开始进行监控
	{
		BOOST_FOREACH(string file_path, watch_list) //添加要监控的文件
		{
			add_watch_dir(file_path);
		}
		struct epoll_event epev;  //记录套接字相关信息
		epev.events = EPOLLIN;    //监视有数据可读事件
		epev.data.fd = inotify_fd;//文件描述符数据，其实这里可以放任何数据。
		int epollfd = epoll_create(1024); //创建 epoll 文件描述符，出错返回-1
		try
		{
			//加入监听列表，当 listenfd 上有对应事件产生时， epoll_wait 会将 epoll_event 填充到 events_in 数组里, 出错返回 -1
			if (epoll_ctl(epollfd, EPOLL_CTL_ADD, inotify_fd, &epev) == -1)
			{
				throw exception() << err_str("inotify_fd add epoll listen fail");
			}
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return false;
		}

		struct epoll_event events_in[16];
		int len, nread;
		char buf[1024];
		struct inotify_event* event;
		while (true)
		{
			int event_count = epoll_wait(epollfd, events_in, sizeof(events_in) - 1, 100); 
			//等待事件， epoll_wait 会将事件填充至 events_in 内
			//返回 获得的事件数量，若超时且没有任何事件返回 0 ，出错返回 -1 。 timeout设置为-1表示无限等待。毫秒级，1秒等于1000毫秒
			for (int i = 0; i < event_count; i++)      //遍历所有事件
			{
				while ((len = read(inotify_fd, buf, sizeof(buf) - 1)) > 0) //从缓冲池中取出事件
				{
					nread = 0;
					while (len > 0)
					{
						event = (struct inotify_event*) & buf[nread];
						if (event->len > 0)
						{
							handle_event(event->wd, event->name, event->mask);
						}
						nread = nread + sizeof(struct inotify_event) + event->len;
						len = len - sizeof(struct inotify_event) - event->len;
					}
				}
			}
		}
		return true;
	}
	void file_system_inotify::handle_event(const int& wd, const string& name, const uint32_t& mask) //处理一个事件
	{
		string parent_name = get_inotifyed_file(wd);
		if (parent_name.empty() == false)
		{
			string file_path = parent_name + "/" + name;
			out->logs << OUTINFO << "event->wd=" << wd << ", event->name=" << file_path << ", event->mask=" << transform_event_mask(mask) << transform_event_mask_chinese(mask);
			//file_op.show_file_info(parent_name + "/" + name);
			if ((mask & IN_ISDIR) && ((mask & IN_CREATE) || (mask & IN_MOVED_TO)) && is_exists_inotifyed_file(file_path) == false) //如果新增加或者移入了一个目录，将其添加到监控列表
			{
				add_watch_unit(file_path);
			}
			else if ((mask & IN_ISDIR) && ((mask & IN_DELETE) || (mask & IN_MOVED_FROM))) //如果删除或者移去了一个目录，将其从监控列表中删除
			{
				int file_wd = get_inotifyed_file(file_path);
				if (file_wd != -1)
				{
					inotify_rm_watch(inotify_fd, file_wd);
					inotify_list.left.erase(file_wd);
					out->logs << OUTINFO << file_path << " del inotifyed success, wd=" << file_wd;
				}
			}
		}
	}
}