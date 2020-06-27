#include "system_action.h"
namespace opensync
{
	system_action::system_action()
	{
	}
	system_action::~system_action()
	{
	}
	string system_action::get_proc_name(const pid_t& pid)
	{
		static pid_t static_pid = -1;
		static string static_proc_name = "";
		try
		{
			if (static_pid != pid)
			{
				char* p = NULL;
				int count = 0;
				char chPath[4096] = { 0 };
				char cParam[4096] = { 0 };

				sprintf(cParam, "/proc/%d/exe", pid);
				count = readlink(cParam, chPath, 4096);
				if (count < 0 || count >= 4096)
				{
					throw exception() << err_str(to_string(pid) + ", Current System not Surport Proc, mesg=" + strerror(errno));
				}
				else
				{
					p = basename(chPath);
					if (p == NULL)
					{
						throw exception() << err_str(to_string(pid) + ", is not exists, mesg=" + strerror(errno));
					}
				}
				static_pid = pid;
				//static_proc_name = basename(chPath);
				static_proc_name = chPath;
			}
		}
		catch (exception& e)
		{
			out->logs << OUTERROR << *boost::get_error_info<err_str>(e);
			return "";
		}
		out->logs << OUTINFO << "pid: " << pid << "的进程名为: " << static_proc_name;
		return static_proc_name;
	}

}