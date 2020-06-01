#include "opensync_public_include_file.h"

#include <map>
#include <boost/version.hpp>
#include <boost/smart_ptr.hpp>

#include "instance_garbo.h"
#include "file_system_operation.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "opensync start" << endl;
	opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();
	out->logs << OUTINFO << "BOOST_VERSION: " << BOOST_VERSION;
	out->logs << OUTINFO << "BOOST_LIB_VERSION: " << BOOST_LIB_VERSION;

	boost::shared_ptr<opensync::file_system_operation> file_op(new opensync::file_system_operation);
	//file_op->get_file_info("/home/wnh/projects/opensync/bin/x64/Debug/opensync.out");
	file_op->show_file_info("/home/wnh/projects/opensync/bin/x64/Debug/opensync.out");

	opensync::instance_garbo garbo = opensync::instance_garbo();
	cout << "opensync end" << endl;
}