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
	}

	return;
}


inline void launch_programs() {
	printf("Launching programs...\n");

	tinydir_dir dir;
	tinydir_open(&dir, "/etc/autostart");

	while (dir.has_next) {
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (!file.is_dir) {
			char* file_name = malloc(256);
			strcat(file_name, "/etc/autostart/");
			strcat(file_name, file.name);
			char* args[]= {file.name, NULL};
			char* env[]={"PATH=/bin:/sbin", NULL};
			printf("Executing %s...\n", file_name);

			pid_t pid = fork();
			
			if (pid == 0) {
				execve(file_name, args, env);
				perror("execve");
			}
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);	

	return;
}
