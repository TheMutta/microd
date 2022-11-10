#pragma once
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <linux/reboot.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <stdlib.h>

#include "state.h"

namespace util {
	struct arguments {
		bool is_in_root;
		bool is_debug;
		char* rootdrv;
		char* rootfstype;
	};
	
	
	void exec(const std::vector<std::string> &argv);
	void w_execvp(const std::string &file, const std::vector<std::string> &argv);
	void ok(std::string message);
	void panic(std::string message);
	void warning(std::string message);
	void debug_shell();
}
