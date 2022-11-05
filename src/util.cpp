#include "util.h"

#include "rootutils.h"

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
	change_state(sys_halt);
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
	}
}

void change_state(change_action action) {
	if (unit::managed_units.size() > 0) {
		for (unsigned long int i = 0; i < unit::managed_units.size(); i++) {
			std::cout << "Sending SIGTERM to " << unit::managed_units[i].pid << std::endl;
			kill(unit::managed_units[i].pid, SIGTERM);
		}

		sleep(2);

		for (unsigned long int i = 0; i < unit::managed_units.size(); i++) {
			if(waitpid(unit::managed_units[i].pid, &unit::managed_units[i].status, WNOHANG) != 0) {
				if (WIFEXITED(unit::managed_units[i].status)) {
					unit::managed_units.erase(unit::managed_units.begin() + i);
				}
			}
		}

		for (unsigned long int i = 0; i < unit::managed_units.size(); i++) {
			std::cout << "Sending SIGKILL to " << unit::managed_units[i].pid << std::endl;
			kill(unit::managed_units[i].pid, SIGKILL);
		}
	}

	sync();
	
	switch (action) {
		default:
		case sys_reboot:
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART, 0);
			break;
		case sys_poweroff:
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_POWER_OFF, 0);
			break;
		case sys_halt:
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_HALT, 0);
			break;
		case sys_suspend:
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_SW_SUSPEND, 0);
			break;
		case sys_kexec:
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_KEXEC, 0);
			break;
		case sys_runlevel_1:
			debug_shell();
		case sys_runlevel_2:
		case sys_runlevel_3:
		case sys_runlevel_4:
		case sys_runlevel_5:
			// Do something...
			root::startup_scripts();
			root::launch_programs();
			break;
	}
}

}
