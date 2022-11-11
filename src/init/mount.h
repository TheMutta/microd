#pragma once
#include <unistd.h>
#include <sys/mount.h>

#include "util.h"

namespace mounting {

        void mount_specialfs();
        void unmount_specialfs();
        
        void remount_root_ro();
        void remount_root_rw(unsigned long rootfs_mount_flags);
        void mount_root(const char* rootfs, const char* rootfs_type, unsigned long rootfs_mount_flags);
        void mount_drive(const char* drv,
                 const char* destdir,
                 const char* fstype,
                 unsigned long flags,
                 std::string warning_message,
                 std::string ok_message);
	
        void unmount_drive(const char* destdir,
                 std::string warning_message,
                 std::string ok_message);


}
