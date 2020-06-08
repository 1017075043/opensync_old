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
	file_op->show_file_info("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_path("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_size("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_type("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_type_name("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_permissions("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_permissions_name("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_last_write_time("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_last_write_time_s("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_user("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_user_name("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_group("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_group_name("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_hash("/tmp/a/200.txt");
	out->logs << OUTINFO << file_op->get_file_status("/tmp/a/200.txt");
	opensync::instance_garbo garbo = opensync::instance_garbo();
	cout << "opensync end" << endl;
}