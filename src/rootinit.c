#include "rootinit.h"

inline void remount_root_rw(char* rootfs, char* rootfs_type, unsigned long rootfs_mount_flags) {
	printf("Remounting root as read-write.\n");
	if (mount(rootfs, "/", rootfs_type, rootfs_mount_flags, "") !=0 ) {
		panic("Failed to remount rootfs. Cannot proceed.");
	} else {
		ok("Rootfs has been remounted");
	}

	return;
}

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
		ok("Done with startup scripts");
	}

	return;
}


inline void launch_daemons() {
	printf("Launch programs...\n");

	

	// In a directory there should be either:
	//  - symlinks to programs
	//  - shell scripts that launch programs
	// that can be directly executed

	/*
	pid_t pid=fork();

	if (pid==0) {
		execve();
		perror("execve");
	} else {
		waitpid(pid,0,0);
		ok("Done launching programs");
	}
	*/

	return;
}
