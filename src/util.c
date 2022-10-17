#include "util.h"

inline void ok(char* message) {
	printf("[\033[0;32mOK\033[0m] %s\n", message);
}

inline void panic(char* message) {
	printf("[\033[0;31mPANIC\033[0m] %s\n", message);
	debug_shell();
	kill(getpid(), SIGUSR1);
}

inline void warning(char* message) {
	printf("[\033[0;33m!!!\033[0m] %s\n", message);
}

inline void debug_shell() {
	warning("Init started debug mode!");
	printf("Executing debug shell.\nTo continue with the init process, exit the shell.\n"
	       "If this comes after a PANIC, after closing the shell the init will send a SIGUSR1 (reboot)!\n");

	pid_t pid=fork();

	if (pid==0) {
		static char *argv[]={"busybox", "setsid", "cttyhack", "sh", NULL};
		static char *env[]={"PATH=/bin:/sbin", NULL};
		execve("/bin/busybox", argv, env);
		perror("execve");
	} else {
		waitpid(pid,0,0);
		ok("Exited debug shell");
	}
}

inline void reboot() {
	sync();
	syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART, 0);
}
