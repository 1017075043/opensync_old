#pragma once
#include "opensync_public_include_file.h"

#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <map>

#include <boost/noncopyable.hpp>

using namespace std;

/*
	linux系统操作类（单例）
	@author 吴南辉
	@time 2020/05/30
*/

namespace opensync
{
	class system_action : boost::noncopyable
	{
	private:
		opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();

	public:
		system_action();
		~system_action();
		string get_proc_name(const pid_t& pid);
	};
}

