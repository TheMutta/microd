#include "rdinit.h"

void initrd_init(util::arguments init_arguments) {
        
        std::cout << " * Starting initrd /init..." << std::endl;
        util::ok();
	
        mounting::mount_fstab();

	if (init_arguments.is_debug) {
		util::debug_shell();
	}
		
        mounting::mount_root(init_arguments.rootdrv, init_arguments.rootfstype, MS_RDONLY);
	switch_root();
}

void switch_root() {
        std::cout << " * Switching to new root on /new_root..." << std::endl;
	if (syscall(SYS_pivot_root, "/new_root", "/") !=0 ) {
		util::panic();
        } else {
		util::ok();
	}
}

