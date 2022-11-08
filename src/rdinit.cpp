#include "rdinit.h"

void initrd_init(util::arguments init_arguments) {
	util::ok("Started initrd /init");
	
        mounting::mount_specialfs();
	util::ok("Finished mounting filesystems.");

	if (init_arguments.is_debug) {
		util::debug_shell();
	}
		
        mounting::mount_root(init_arguments.rootdrv, init_arguments.rootfstype, MS_RDONLY | MS_NOATIME);
	switch_root();
}

void switch_root() {
	printf("Switching to new root on /new_root\n");
	if (syscall(SYS_pivot_root, "/new_root", "/") !=0 ) {
		util::panic("Failed to pivot_root on /new_root. Cannot proceed.");
	} else {
		util::ok("Root has been switched");
	}
}

