#pragma once
#include <sys/mount.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "mount.h"
#include "util.h"

void initrd_init(util::arguments init_arguments);

void mount_root(char* rootfs, char* rootfs_type, unsigned long rootfs_mount_flags);
void switch_root();

