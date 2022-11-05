#pragma once
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <linux/reboot.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <stdlib.h>

#include "unit.h"

namespace util {
	struct arguments {
		bool is_in_root;
		bool is_debug;
		char* rootdrv;
		char* rootfstype;
	};

	enum change_action { sys_reboot,
			     sys_poweroff,
			     sys_halt,
			     sys_suspend,
			     sys_kexec,
			     sys_runlevel_1,
			     sys_runlevel_2,
			     sys_runlevel_3,
			     sys_runlevel_4,
			     sys_runlevel_5,
	};

	void exec(const std::vector<std::string> &argv);
	void w_execvp(const std::string &file, const std::vector<std::string> &argv);
	void ok(std::string message);
	void panic(std::string message);
	void warning(std::string message);
	void debug_shell();
	void change_state(change_action action);
}
