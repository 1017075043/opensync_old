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
	//file_inotify_thread.join(); //�̻߳���

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
	file_listen_thread.join(); //�̻߳���


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
//	    //ѡ�������
//	    void setNotifyObject(std::string path);
//	    //��ȡ���
//	    int getNotifyFD();
//	    //��fanotify����ӵ�select�п�ʼ����
//	    void startListen();
//	    //���ô򿪺͹ر��¼�
//	    void detectOpenClose();
//	    //���ü���޸��¼�
//	    void detectWrite();
//	    //�ڼ���ڼ���Բ����ļ�
//	    void operationFile(int fd);
//	    //��ʼ��������
//	private:
//	    std::string paths;
//	    //��һЩ������ͼ���ļ���ʱ��fanotify��⵽�����ں˷���������ʱ�־����ȻҲ���Է��Ͳ�������ʱ�־
//	    int handlePerm(const struct fanotify_event_metadata* metadata);
//	    //select����¼�
//	    int selectEvent(fd_set* rfd);
//	    //��ȡ�ļ������¼�����
//	    int getEvent(const struct fanotify_event_metadata* metadata, int len);
//	    //���ü������
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
//	     std::cout << "ʹ���ṩ��ϵͳ���ã����ÿ��ر�־" << std::endl;
//	    int ret = fanotify_mark(fanFd, FAN_MARK_ADD, FAN_CLOSE | FAN_OPEN, AT_FDCWD, paths.c_str());
//	    if (ret < 0) {
//	         std::cout << __FILE__ << "    " << __LINE__ << std::endl;
//	    }
//	}
//	
//	//���ü�ض���ΪĿ¼�µ����ļ�
//	void Fanotify::setNotifyObject(std::string path) {
//	    this->paths = path;
//	    //��ʼ������һ������ΪFAN_CLASS_CONTENT����ʾ�������֪ͨ�ļ��¼�����һ��ֵΪFAN_CLASS_NOTIF ΪĬ��ֵ��ֻ��������¼�֪ͨ����һ��ļ����У�ʹ��FAN_CLASS_CONTENT
//	    //�ڶ�����������ʾ���յ������ļ��¼��󣬱�������ļ��Ĳ���Ϊ�ɶ���д����Ϊmetadata�е�fdΪ������������¼����ļ���fd������ֱ�Ӳ����Ǹ��ļ����������Ҫ�ص���
//	    fanFd = fanotify_init(FAN_CLASS_CONTENT, O_RDWR);
//	    if (fanFd < 0) {
//	         std::cout << __FILE__ << "   " << __LINE__ << "     " << strerror(errno) << std::endl;
//	        return;
//	    }
//	     std::cout << "��ʼ�����" << std::endl;
//	    uint64_t fanMask = 0;
//	    //�������ļ�open��ʱ�򣬻�֪ͨ������ͬ����ʵĻ�����������Ҫ���ں˷�������ʱ�־��Ȼ������������ܼ������ʣ������ܷ���
//	    fanMask |= FAN_OPEN_PERM;
//	    fanMask |= FAN_ALL_PERM_EVENTS;
//	    //FAN_EVENT_ON_CHILD �����ڵ�ǰ����Ŀ¼���������ļ�
//	    //FAN_MARK_ADD ��Ӽ����¼���־�ı�־
//	    int ret = fanotify_mark(fanFd, FAN_MARK_ADD, fanMask | FAN_EVENT_ON_CHILD | FAN_MOVE,
//	        AT_FDCWD, path.c_str());
//	    if (ret < 0) {
//	         std::cout << __LINE__ << "   " __FILE__ << "   " << strerror(errno) << std::endl;
//	    }
//	     std::cout << "���������" << std::endl;
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
//	    //ʹ��select����
//	    FD_ZERO(&rfd);
//	    FD_SET(fanFd, &rfd);
//	     std::cout << "��ʼ����" << std::endl;
//	    selectEvent(&rfd);
//	     std::cout << "�����¼�" << std::endl;
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
//	             std::cout << "������" << std::endl;
//	        }
//	        selectEvent(&rfd);
//	    }
//	     std::cout << strerror(errno) << std::endl;
//	}
//	
//	int Fanotify::selectEvent(fd_set* rfd) {
//	     std::cout << "����" << std::endl;
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
//	        //����matadata
//	        if (metadata->mask & FAN_OPEN) {
//	             std::cout << "�ļ�����" << std::endl;
//	            int fd = metadata->fd;
//	            /*const char* buf = "hello,it's a secret!" ;
//	            write(fd, buf, strlen(buf)) ;*/
//	        }
//	        if (metadata->mask & FAN_CLOSE) {
//	            if (metadata->mask & FAN_CLOSE_WRITE) {
//	                 std::cout << "д�ر�" << std::endl;
//	                return CLOSE_MODIFY;
//	            }
//	            if (metadata->mask & FAN_CLOSE_NOWRITE) {
//	                 std::cout << "�رղ���" << std::endl;
//	            }
//	        }
//	        if (metadata->mask & FAN_MODIFY) {
//	             std::cout << "�޸�" << std::endl;
//	        }
//	        if (metadata->mask & FAN_OPEN_PERM) {
//	             std::cout << "open perm" << std::endl;
//	        }
//	        if (metadata->mask & FAN_ALL_PERM_EVENTS) {
//	             std::cout << "���ں˷�����Ϣ" << std::endl;
//	            handlePerm(metadata);
//	            //�����ļ�
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
//	         std::cout << "дʧ��" << std::endl;
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