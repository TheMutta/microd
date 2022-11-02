#include "rdinit.h"

inline void mount_root(char* rootfs, char* rootfs_type, unsigned long rootfs_mount_flags) {
	printf("Mounting %s as a %s root device.\n", rootfs, rootfs_type);
	if (mount(rootfs, "/new_root", rootfs_type, rootfs_mount_flags, "") !=0 ) {
		panic("Failed to mount rootfs on /new_root. Cannot proceed.");
	} else {
		ok("Rootfs has been mounted on /new_root");
	}

	return;
}

inline void switch_root() {
	printf("Switching to new root on /new_root\n");
	if (syscall(SYS_pivot_root, "/new_root", "/") !=0 ) {
		panic("Failed to pivot_root on /new_root. Cannot proceed.");
	} else {
		ok("Root has been switched");
	}
}

