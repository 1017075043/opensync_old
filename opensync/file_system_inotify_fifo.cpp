#include "file_system_inotify.h"
namespace opensync
{
	void file_system_inotify::inotify_thread_s(const int& thread_num)
	{
		try
		{
			//设置线程与CPU亲和力
			cpu_set_t mask;  //CPU核的集合
			cpu_set_t get;   //获取在集合中的CPU
			CPU_ZERO(&mask);    //置空
			CPU_SET(thread_num, &mask);   //设置亲和力值
			if (sched_setaffinity(0, sizeof(mask), &mask) == -1)//设置线程CPU亲和力
			{
				throw exception() << err_str("thread=" + to_string(thread_num) + ", could not set CPU affinity, " + strerror(errno));
			}
			CPU_ZERO(&get);
			if (sched_getaffinity(0, sizeof(get), &get) == -1)//获取线程CPU亲和力
			{
				throw exception() << err_str("thread=" + to_string(thread_num) + ", cound not get thread affinity, " + strerror(errno));
			}
			if (!CPU_ISSET(thread_num, &get))//判断线程与哪个CPU有亲和力
			{
				throw exception() << err_str("thread=" + to_string(thread_num) + ", this thread " + to_string(thread_num) + " is running processor :" + to_string(thread_num) + ", " + strerror(errno));
			}
			out->logs << OUTINFO << "thread=" << thread_num << ", this thread" << thread_num << " is running processor:" << thread_num;
			
			//设置线程优先级为99
			struct sched_param param;
			int sched_method = 1;
			int sched_priority = 99;
			param.sched_priority = sched_priority;
			if (sched_setscheduler(0, sched_method, &param))
			{
				throw exception() << err_str("thread=" + to_string(thread_num) + ", set scheduler to 1 99 failed, " + strerror(errno));
			}
			int scheduler = sched_getscheduler(0);
			out->logs << OUTINFO << "thread=" << thread_num << ", the scheduler of " << thread_num << " is " << scheduler << ", priority " << sched_priority;
			if (thread_num != 0)
			{
				boost::this_thread::sleep(boost::posix_time::seconds(3));
				out->logs << OUTINFO << "thread=" << thread_num << ", inotify thread start.";
				open_inotify_stake();
			}
			else
			{
				out->logs << OUTINFO << "thread=" << thread_num << ", inotify thread start.";
				open_inotify();
			}
			out->logs << OUTINFO << "thread=" << thread_num << ", inotify thread stop.";
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
		}
	}
	void file_system_inotify::start_watch() //创建和CPU相同的线程数，
	{
		int cpu_num = sysconf(_SC_NPROCESSORS_CONF);  //获取CPU核数
		boost::thread inotify_thread[cpu_num];
		out->logs << OUTINFO << "system has " << cpu_num << " processor(s).";
		for (int i = 0; i < cpu_num; i++)
		{
			inotify_thread[i] = boost::thread(boost::bind(&file_system_inotify::inotify_thread_s, this, i));
		}
		for (int i = 0; i < cpu_num; ++i)
		{
			inotify_thread[i].join();
		}
		return;
	}

	void file_system_inotify::shop_watch() //停止监听
	{
		run_flag = false;
	}
}