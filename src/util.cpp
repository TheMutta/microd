#include "util.h"

void ok(std::string message) {
	std::cout << "[\033[0;32mOK\033[0m] " << message << std::endl;
}

void panic(std::string message) {
	std::cout << "[\033[0;31mPANIC\033[0m] " << message << std::endl;
	debug_shell();
	reboot();
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

void reboot() {
	sync();
	syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART, 0);
}
