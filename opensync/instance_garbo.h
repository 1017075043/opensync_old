#pragma once

#include "opensync_public_include_file.h"
#include "file_info_databases.h"
#include "user_group_info.h"

/*
	��������
	@author ���ϻ�
	@time 2020/05/31
*/

namespace opensync
{
	class instance_garbo
	{
	private:
		opensync::log4cpp_instance* out = opensync::log4cpp_instance::init_instance();
	public:
		instance_garbo();
		~instance_garbo();
	};
}

