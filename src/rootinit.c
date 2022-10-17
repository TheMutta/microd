#include "rootinit.h"

inline void mount_drive(char* drv, char* fstype, char* destdir, unsigned long flags) {
	printf("Mounting %s in %s...\n", drv, destdir);
	if (mount(drv, destdir, fstype, flags, "") !=0 ) {
		warning("Failed to mount! The system may not behave correctly.");
	} else {
		ok("Drive mounted");
	}

	return;
}

inline void startup_scripts() {
	printf("Running startup scripts...\n");

	pid_t pid=fork();

	if (pid==0) {
		static char *argv[]={"busybox", "sh", "/etc/inittab", NULL};
		static char *env[]={"PATH=/bin:/sbin", NULL};
		execve("/bin/busybox", argv, env);
		perror("execve");
	} else {
		waitpid(pid,0,0);
		ok("Exited debug shell");
	}

	return;
}

inline void launch_daemons() {
	printf("Launch programs...\n");

	return;
}
