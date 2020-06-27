#include "opensync_public_include_file.h"

#include <map>
#include <boost/version.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "instance_garbo.h"
#include "file_system_operation.h"
#include "file_system_inotify.h"
#include "file_system_fanotify.h"
#include "file_system_listen.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "opensync start" << endl;
	opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();
	out->logs << OUTINFO << "BOOST_VERSION: " << BOOST_VERSION;
	out->logs << OUTINFO << "BOOST_LIB_VERSION: " << BOOST_LIB_VERSION;
	//opensync::file_system_fanotify* file_fanotify = opensync::file_system_fanotify::init_instance();
	//file_fanotify->add_watch_unit("/tmp/a");
	//file_fanotify->add_watch_unit("/tmp/a/a");
	//file_fanotify->start_watch();

	//opensync::file_system_inotify* file_inotify = opensync::file_system_inotify::init_instance();
	//file_inotify->add_watch(argv[1]);
	//file_inotify->add_ignore_watch(argv[2]);
	//boost::thread file_inotify_thread(boost::bind(&opensync::file_system_inotify::start_watch, file_inotify));
	//file_inotify->start_watch();
	//for (int i = 0; i < 100; i++)
	//{
	//	out->logs << OUTDEBUG << i;
	//	sleep(1);
	//}
	//file_inotify->shop_watch();
	//file_inotify_thread.join(); //线程回收

	opensync::file_system_listen* file_listen = opensync::file_system_listen::init_instance();
	file_listen->add_watch(argv[1]);
	file_listen->add_ignore_watch(argv[2]);
	boost::thread file_listen_thread(boost::bind(&opensync::file_system_listen::start_watch, file_listen));
	for (int i = 0; i < 1000; i++)
	{
		//out->logs << OUTDEBUG << i;
		sleep(1);
	}
	file_listen->shop_watch();
	file_listen_thread.join(); //线程回收


	opensync::instance_garbo garbo = opensync::instance_garbo();
	cout << "opensync end" << endl;
}


//	#pragma once
//	#include <errno.h>
//	#include <inttypes.h>
//	#include <fcntl.h>
//	#include <linux/limits.h>
//	#include <iostream>
//	#include <signal.h>
//	#include <stdbool.h>
//	#include <stdio.h>
//	#include <stdlib.h>
//	#include <memory>
//	#include <string.h>
//	#include <sys/select.h>
//	#include <sys/time.h>
//	#include <sys/types.h>
//	#include <sys/stat.h>
//	#include <unistd.h>
//	#include <string.h>
//	#include <sys/fanotify.h>
//	
//	#include <unistd.h>
//	#include <linux/types.h>
//	
//	
//	const int OPEN = 1;
//	const int MODIFY = 2;
//	const int CLOSE_MODIFY = 3;
//	const int CLOSE = 4;
//	
//	class Fanotify {
//	public:
//	    Fanotify();
//	    ~Fanotify();
//	    //选择检测对象
//	    void setNotifyObject(std::string path);
//	    //获取句柄
//	    int getNotifyFD();
//	    //将fanotify句柄加到select中开始监听
//	    void startListen();
//	    //设置打开和关闭事件
//	    void detectOpenClose();
//	    //设置检测修改事件
//	    void detectWrite();
//	    //在监控期间可以操作文件
//	    void operationFile(int fd);
//	    //开始监听函数
//	private:
//	    std::string paths;
//	    //当一些程序试图打开文件的时候，fanotify检测到病给内核发送允许访问标志，当然也可以发送不允许访问标志
//	    int handlePerm(const struct fanotify_event_metadata* metadata);
//	    //select监测事件
//	    int selectEvent(fd_set* rfd);
//	    //获取文件操作事件类型
//	    int getEvent(const struct fanotify_event_metadata* metadata, int len);
//	    //设置检测对象的
//	 //   std::shared_ptr<epOperation>ep ;
//	    int fanFd;
//	};
//	
//	Fanotify::Fanotify() {
//	}
//	
//	Fanotify :: ~Fanotify() {
//	    close(fanFd);
//	}
//	
//	void Fanotify::detectOpenClose() {
//	     std::cout << "使用提供的系统调用，设置开关标志" << std::endl;
//	    int ret = fanotify_mark(fanFd, FAN_MARK_ADD, FAN_CLOSE | FAN_OPEN, AT_FDCWD, paths.c_str());
//	    if (ret < 0) {
//	         std::cout << __FILE__ << "    " << __LINE__ << std::endl;
//	    }
//	}
//	
//	//设置监控对象为目录下的子文件
//	void Fanotify::setNotifyObject(std::string path) {
//	    this->paths = path;
//	    //初始化，第一个参数为FAN_CLASS_CONTENT（表示允许接收通知文件事件）另一个值为FAN_CLASS_NOTIF 为默认值。只允许接收事件通知，在一般的监听中，使用FAN_CLASS_CONTENT
//	    //第二个参数，表示接收到操作文件事件后，本程序对文件的操作为可读可写，因为metadata中的fd为程序操作发生事件的文件的fd，可以直接操作那个文件，操作完后，要关掉。
//	    fanFd = fanotify_init(FAN_CLASS_CONTENT, O_RDWR);
//	    if (fanFd < 0) {
//	         std::cout << __FILE__ << "   " << __LINE__ << "     " << strerror(errno) << std::endl;
//	        return;
//	    }
//	     std::cout << "初始化完成" << std::endl;
//	    uint64_t fanMask = 0;
//	    //设置在文件open的时候，会通知本程序，同意访问的话，本程序需要给内核发允许访问标志，然后其他程序才能继续访问，否则不能访问
//	    fanMask |= FAN_OPEN_PERM;
//	    fanMask |= FAN_ALL_PERM_EVENTS;
//	    //FAN_EVENT_ON_CHILD 作用于当前操作目录的所有子文件
//	    //FAN_MARK_ADD 添加监听事件标志的标志
//	    int ret = fanotify_mark(fanFd, FAN_MARK_ADD, fanMask | FAN_EVENT_ON_CHILD | FAN_MOVE,
//	        AT_FDCWD, path.c_str());
//	    if (ret < 0) {
//	         std::cout << __LINE__ << "   " __FILE__ << "   " << strerror(errno) << std::endl;
//	    }
//	     std::cout << "对象处理完成" << std::endl;
//	}
//	
//	int Fanotify::getNotifyFD() {
//	    return fanFd;
//	}
//	
//	void Fanotify::startListen() {
//	    char buf[4096];
//	    int len = 0;
//	    fd_set rfd;
//	    //使用select监听
//	    FD_ZERO(&rfd);
//	    FD_SET(fanFd, &rfd);
//	     std::cout << "开始监听" << std::endl;
//	    selectEvent(&rfd);
//	     std::cout << "发生事件" << std::endl;
//	    while ((len = read(fanFd, buf, sizeof(buf))) > 0) {
//	        struct fanotify_event_metadata* metadata;
//	        char path[PATH_MAX];
//	        int pathLen;
//	        metadata = (fanotify_event_metadata*)buf;
//	        if (metadata->fd >= 0) {
//	            sprintf(path, "/proc/self/fd/%d", metadata->fd);
//	            pathLen = readlink(path, path, sizeof(path) - 1);
//	            if (pathLen < 0) {
//	                 std::cout << __LINE__ << "     " << __FILE__ << std::endl;
//	                exit(1);
//	            }
//	            path[pathLen] = '\0';
//	            getEvent(metadata, len);
//	             std::cout << "检测完成" << std::endl;
//	        }
//	        selectEvent(&rfd);
//	    }
//	     std::cout << strerror(errno) << std::endl;
//	}
//	
//	int Fanotify::selectEvent(fd_set* rfd) {
//	     std::cout << "阻塞" << std::endl;
//	    while (select(fanFd + 1, rfd, NULL, NULL, NULL) < 0) {
//	        if (errno != EINTR) {
//	             std::cout << __LINE__ << "     " << std::endl;
//	            exit(0);
//	        }
//	    }
//	    return 1;
//	}
//	
//	int Fanotify::getEvent(const struct fanotify_event_metadata* metadata, int len) {
//	    std::string paths;
//	    while (FAN_EVENT_OK(metadata, len)) {
//	        //处理matadata
//	        if (metadata->mask & FAN_OPEN) {
//	             std::cout << "文件被打开" << std::endl;
//	            int fd = metadata->fd;
//	            /*const char* buf = "hello,it's a secret!" ;
//	            write(fd, buf, strlen(buf)) ;*/
//	        }
//	        if (metadata->mask & FAN_CLOSE) {
//	            if (metadata->mask & FAN_CLOSE_WRITE) {
//	                 std::cout << "写关闭" << std::endl;
//	                return CLOSE_MODIFY;
//	            }
//	            if (metadata->mask & FAN_CLOSE_NOWRITE) {
//	                 std::cout << "关闭操作" << std::endl;
//	            }
//	        }
//	        if (metadata->mask & FAN_MODIFY) {
//	             std::cout << "修改" << std::endl;
//	        }
//	        if (metadata->mask & FAN_OPEN_PERM) {
//	             std::cout << "open perm" << std::endl;
//	        }
//	        if (metadata->mask & FAN_ALL_PERM_EVENTS) {
//	             std::cout << "给内核发送消息" << std::endl;
//	            handlePerm(metadata);
//	            //操作文件
//	            operationFile(metadata->fd);
//	            detectOpenClose();
//	        }
//	        metadata = FAN_EVENT_NEXT(metadata, len);
//	    }
//	    return 1;
//	}
//	
//	void Fanotify::operationFile(int fd) {
//	    const char* buf = "okokokokokokokok";
//	    int ret = write(fd, buf, strlen(buf));
//	    if (ret < 0) {
//	         std::cout << "写失败" << std::endl;
//	    }
//	    close(fd);
//	}
//	
//	int Fanotify::handlePerm(const struct fanotify_event_metadata* metadata) {
//	    struct fanotify_response response_struct;
//	    int ret;
//	    response_struct.fd = metadata->fd;
//	    response_struct.response = FAN_ALLOW;
//	
//	    ret = write(fanFd, &response_struct, sizeof(response_struct));
//	    if (ret < 0)
//	        return ret;
//	
//	    return 0;
//	}
//	
//	
//	#include <iostream>
//	int main(int argc, char** argv) {
//	    for (int i = 0; i < argc; i++) {
//	         std::cout << "[" << __FILE__ << "] [" << __func__ << "] [" << __LINE__ << "] " << argv[i] << std::endl;
//	    }
//	
//	    Fanotify notify;
//	    notify.setNotifyObject(argv[1]);
//	    notify.startListen();
//	    return 0;
//	}