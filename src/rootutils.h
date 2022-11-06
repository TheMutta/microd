#pragma once
#include <sys/mount.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>

#include "util.h"
#include "unit.h"
#include "tinydir.h"
#include "mount_special.h"

namespace root {
	void remount_root_rw(char* rootfs, char* rootfs_type, unsigned long rootfs_mount_flags);
	void mount_drive(char* drv, char* fstype, char* destdir, unsigned long flags);
	void startup_scripts();
	void launch_programs(util::runlevel level); 
}
