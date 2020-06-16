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

using namespace std;

int main(int argc, char* argv[])
{
	cout << "opensync start" << endl;
	opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();
	out->logs << OUTINFO << "BOOST_VERSION: " << BOOST_VERSION;
	out->logs << OUTINFO << "BOOST_LIB_VERSION: " << BOOST_LIB_VERSION;

	opensync::file_system_inotify* file_inotify = opensync::file_system_inotify::init_instance();
	file_inotify->add_watch(argv[1]);
	file_inotify->add_ignore_watch(argv[2]);
	boost::thread file_inotify_thread(boost::bind(&opensync::file_system_inotify::start_watch, file_inotify));
	//file_inotify->start_watch();
	for (int i = 0; i < 10; i++)
	{
		out->logs << OUTDEBUG << i;
		sleep(1);
	}
	file_inotify->shop_watch();
	file_inotify_thread.join(); //线程回收


	opensync::instance_garbo garbo = opensync::instance_garbo();
	cout << "opensync end" << endl;
}