#include "opensync_public_include_file.h"

#include <map>
#include <boost/version.hpp>
#include <boost/smart_ptr.hpp>

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
	file_inotify->open_inotify();

	opensync::instance_garbo garbo = opensync::instance_garbo();
	cout << "opensync end" << endl;
}