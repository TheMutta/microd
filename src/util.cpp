#include "util.h"

namespace util {

void exec(const std::vector<std::string> &argv) {
	if (argv.size() == 0) { 
		throw std::invalid_argument("At least one argument must be provided, being the file of "
				            "the binary to execute");
	}
	
	w_execvp(argv[0], argv);
}

void w_execvp(const std::string &file, const std::vector<std::string> &argv) {
	std::vector<char *> vec_cp;
	vec_cp.reserve(argv.size() + 1);

	for (auto const& s : argv)
		vec_cp.push_back(const_cast<char*>(s.c_str()));

	vec_cp.push_back(nullptr);

	execvp(file.c_str(), vec_cp.data());

	perror("execvp");

}

void ok(std::string message) {
	std::cout << "[\033[0;32mOK\033[0m] " << message << std::endl;
}

void panic(std::string message) {
	std::cout << "[\033[0;31mPANIC\033[0m] " << message << std::endl;
	debug_shell();
        state::change_state(state::sys_halt);
}

void warning(std::string message) {
	std::cout << "[\033[0;33m!!!\033[0m] " << message << std::endl;
}

void debug_shell() {
	warning("Init started debug mode!");
	printf("Executing debug shell.\nTo continue with the init process, exit the shell.\n"
	       "If this comes after a PANIC, after closing the shell the init will send a SIGUSR1 (reboot)!\n");

	pid_t shell_pid = fork();

	if (shell_pid == 0) {
		char *argv[] = {"busybox", "setsid", "cttyhack", "sh", NULL};
		char *env[] = {"PATH=/bin:/sbin", NULL};
		execve("/bin/busybox", argv, env);
		perror("execve");
		exit(1);
	} else {
		waitpid(shell_pid, 0, 0);
		ok("Exited debug shell");
                state::change_state(state::sys_runlevel_2);
	}
}

}
