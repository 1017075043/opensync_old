#include "file_system_listen.h"
namespace opensync
{
	void file_system_listen::inotify_thread_set(const int& thread_num)  //获取线程执行的CPU号
	{
		try
		{
			//设置线程与CPU亲和力
			cpu_set_t mask;  //CPU核的集合
			//cpu_set_t get;   //获取在集合中的CPU
			CPU_ZERO(&mask);    //置空
			CPU_SET(thread_num, &mask);   //设置亲和力值
			if (sched_setaffinity(0, sizeof(mask), &mask) == -1)//设置线程CPU亲和力
			{
				throw exception() << err_str("thread_num=" + to_string(thread_num) + ", could not set CPU affinity, " + strerror(errno));
			}
			//get_inotify_thread_id(thread_num);

			//CPU_ZERO(&get);
			//if (sched_getaffinity(0, sizeof(get), &get) == -1)//获取线程CPU亲和力
			//{
			//	throw exception() << err_str("thread=" + to_string(thread_num) + ", cound not get thread affinity, " + strerror(errno));
			//}
			//if (!CPU_ISSET(thread_num, &get))//判断线程与哪个CPU有亲和力
			//{
			//	throw exception() << err_str("thread=" + to_string(thread_num) + ", this thread " + to_string(thread_num) + " is not running processor :" + to_string(thread_num) + ", " + strerror(errno));
			//}
			//out->logs << OUTINFO << "thread=" << thread_num << ", this thread" << thread_num << " is running processor:" << thread_num;
			
			//设置线程优先级为99
			struct sched_param param;
			int sched_method = 1;
			int sched_priority = 99;
			param.sched_priority = sched_priority;
			if (sched_setscheduler(0, sched_method, &param))
			{
				throw exception() << err_str("thread_num=" + to_string(thread_num) + ", set scheduler to 1 99 failed, " + strerror(errno));
			}
			int scheduler = sched_getscheduler(0);
			out->logs << OUTDEBUG << "thread_num=" << thread_num << ", the scheduler of " << thread_num << " is " << scheduler << ", priority " << sched_priority;
			if (thread_num != 0)
			{
				boost::this_thread::sleep(boost::posix_time::seconds(3));
				out->logs << OUTINFO << "thread_num=" << thread_num << ", inotify thread start.";
				open_inotify_stake();
			}
			else
			{
				out->logs << OUTINFO << "thread_num=" << thread_num << ", inotify thread start.";
				open_inotify();
			}
			out->logs << OUTINFO << "thread_num=" << thread_num << ", inotify thread stop.";
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
		}
	}
	const int file_system_listen::get_inotify_thread_id(const int& thread_num) //获取线程执行的CPU号
	{
		try
		{
			int cpu_num = sysconf(_SC_NPROCESSORS_CONF);  //获取CPU核数
			cpu_set_t get;   //获取在集合中的CPU
			CPU_ZERO(&get);
			if (sched_getaffinity(0, sizeof(get), &get) == -1)//获取线程CPU亲和力
			{
				throw exception() << err_str("thread_num=" + to_string(thread_num) + ", cound not get thread affinity, " + strerror(errno));
			}
			for (int i = 0; i < cpu_num; ++i)
			{
				if (CPU_ISSET(i, &get))//判断线程与哪个CPU有亲和力
				{
					out->logs << OUTDEBUG << "thread_num=" << thread_num << ", is running processor:" << i;
					return i;
				}
			}
			throw exception() << err_str("thread_num=" + to_string(thread_num) + ", is not running processor :" +  strerror(errno));
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
		}
		return -1;
	}
	void file_system_listen::start_watch() //创建和CPU相同的线程数，
	{
		boost::thread fanotify_thread = boost::thread(boost::bind(&file_system_listen::open_fanotify, this)); //创建fanotify线程

		int cpu_num = sysconf(_SC_NPROCESSORS_CONF);  //获取CPU核数
		boost::thread inotify_thread[cpu_num];
		out->logs << OUTINFO << "system has " << cpu_num << " processor(s).";
		for (int i = 0; i < cpu_num; i++)
		{
			out->logs << OUTINFO << "i=" << i << ", 开始尝试创建线程.";
			inotify_thread[i] = boost::thread(boost::bind(&file_system_listen::inotify_thread_set, this, i)); //创建inotify线程
		}
		fanotify_thread.join();
		for (int i = 0; i < cpu_num; ++i)
		{
			inotify_thread[i].join();
		}
		return;
	}
	void file_system_listen::shop_watch() //停止监听
	{
		run_flag = false;
	}
}

/*
fs.inotify.max_user_instances = 128
fs.inotify.max_user_watches = 8192000
fs.inotify.max_queued_events = 1638400
/proc/sys/fs/inotify/max_queued_events

最近在做一个linux上的文件监控程序，2.6内核提供了inotify机制，这仅仅是个机制，任何策略都必须自己实现，
这一点从inotify不提供递归接口就可以看出来，如果我实时监控到目录被创建，那么马上将这个新目录加入监控表，
这个想法是最初的想法，也是最直接的想法，可是仔细推敲一下就会发现这个实现有问题，
比如在检测到目录被创建到新目录添加到监控表的时间间隔内，新的子目录的文件事件以及目录事件将被遗漏，而且会像丝袜脱丝一样一发不可收拾，
新子目录内又创建了一个目录没有被监控到，那么这个子子目录内的事件将递归的丢失，
看来这个事情很严重，那么有没有办法呢？前面说了一种补救的办法，可是难度太大，没有必要，
仔细想想这种丢失并不是频繁发生，只有在像cp -r或者tar快速创建目录时才会发生，既然我们没有办法实现补救方案，
那么可以从进程执行这个大框架入手，如果我们可以让cp或者tar在监控程序加入新目录之前不执行就可以了，
于是可以通过优先级来实现，将监控程序设置为实时FIFO优先级就可以了。当文件系统的系统调用执行完，
inotify开始执行的时候，最后会wake up等待inotify描述符的监控进程，而在系统调用返回用户空间的时候会检查need_sched标志位，
因为监控进程是实时调度类，优先级是很高的，因此必定会抢占当前的文件操作的进程，
可是在多cpu上怎么能保证这个文件操作进程不被调度到别的cpu上呢？说实话，不能，于是有了下面的解决方案。
文件同步方案已经找到，还是用inotify，利用inotify-tools工具的inotifywait程序对目录进行监控，
并且实时加入新创建的子目录，为了避免遗漏，我的做法是：
单cpu方案：
解决办法：将监控进程的优先级设置为FIFO实时优先级，根据inotify的内核实现和2.6内核的进程调度原理（根据是2.6.X的内核源代码），
实时优先级的监控进程总是可以在新子目录创建文件前首先加入该子目录，这样就不会遗漏了。
多cpu方案：
问题：因为在多cpu的情况下，即使将监控进程设置为FIFO的实时进程，那么还是可能将cp -r或者tar等快速创建子目录和文件的进程调度到别的cpu，
从而和我们的监控进程构成竞争最终造成事件遗漏。
解决办法：将监控进程分解为多个线程，每个cpu绑定一个线程，这些线程共享一个inotify描述符，这样就不会造成读取的事件重复。
如此一来，在新目录被添加以后，每个cpu上的均会运行实时FIFO线程，从而把任何非实时进程的执行拦截。在多cpu上，
实际只要有一个文件操作，就会唤醒所有cpu上的监控进程，这是靠ipi（处理器间中断）实现的。
效果：经过测试，发现没有遗漏。
仍然具有的问题：从内核源代码来看，如果没有将内核编译成内核抢占，那么还是有可能遗漏，只不过这种可能性非常之小，
我用tar和cp -r没有测试出来。
虽然每个cpu一个监控进程解决了大致框架问题，但是又引入了新的问题，怎么处理这么多的进程间的通信，
inotifywait是用红黑树实现的文件索引，那么多的线程肯定会打乱红黑树的，于是又有了新的想法。
想想看设置多个线程，每个cpu一个线程的原因就是靠这些线程的优先级是实时FIFO来阻止新目录加入监控表前的文件操作，
于是我们只要保证一个cpu上进行实际工作，别的cpu上的线程不做任何监控，只是一个桩就可以了，现在问题就是这个桩怎么设计，
很简单，办法有两个，一个就是在别的cpu的线程随便实现一个无限的等待循环，
另一个方案就是在别的cpu上执行inotify描述符的select而不做read，这种方案一定可以，
相信我没错的。
*/