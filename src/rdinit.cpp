#include "rdinit.h"

void mount_root(char* rootfs, char* rootfs_type, unsigned long rootfs_mount_flags) {
	std::cout << "Mounting " << rootfs << " as a " << rootfs_type << " root device." << std::endl;
	if (mount(rootfs, "/new_root", rootfs_type, rootfs_mount_flags, "") !=0 ) {
		util::panic("Failed to mount rootfs on /new_root. Cannot proceed.");
	} else {
		util::ok("Rootfs has been mounted on /new_root");
	}

	return;
}

void switch_root() {
	printf("Switching to new root on /new_root\n");
	if (syscall(SYS_pivot_root, "/new_root", "/") !=0 ) {
		util::panic("Failed to pivot_root on /new_root. Cannot proceed.");
	} else {
		util::ok("Root has been switched");
	}
}

