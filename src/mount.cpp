#include "mount.h"

namespace mounting {

void mount_drive(const char* drv,
                 const char* destdir,
                 const char* fstype,
                 unsigned long flags,
                 std::string warning_message="Failed to mount! The system may not behave correctly.",
                 std::string ok_message="Drive mounted.") {
	std::cout << "Mounting " << drv << " " << fstype << " in " << destdir << "..." << std::endl;

	if (mount(drv, destdir, fstype, flags, "") !=0 ) {
		util::warning(warning_message);
	} else {
		util::ok(ok_message);
	}

	return;
}

void unmount_drive(const char* destdir,
                 std::string warning_message="Failed to unmount! The system may not behave correctly.",
                 std::string ok_message="Drive unmounted.") {
	std::cout << "Unounting fs in " << destdir << "..." << std::endl;

        if (umount(destdir) != 0) {
	       	util::warning(warning_message);
	} else {
		util::ok(ok_message);
	}

	return;
}


void mount_specialfs() {
	std::cout << "Mounting filesystems..." << std::endl;

        mount_drive("none", "/dev", "devtmpfs", MS_NOSUID,
                    "Failed to mount /dev. The system may not behave correctly.",
                    "/dev mounted.");

        mount_drive("none", "/sys", "sysfs",
                    MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_NOSUID | MS_RELATIME,
                    "Failed to mount /sys. The system may not behave correctly.",
                    "/sys mounted.");

	mount_drive("none", "/proc", "proc",
                    MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_NOSUID | MS_RELATIME,
                    "Failed to mount /proc. The system may not behave correctly.",
                    "/proc mounted.");

	mount_drive("none", "/tmp", "tmpfs",
                    MS_NOATIME | MS_NODEV | MS_NOSUID,
                    "Failed to mount /tmp. The system may not behave correctly.",
                    "/tmp mounted.");

	return;
}

void unmount_specialfs() {
        std::cout << "Unmounting filesystems..." << std::endl;
	
        std::cout << "Unmounting tmpfs in /tmp..." << std::endl;
	unmount_drive("/tmp",
                    "Failed to unmount /tmp. The system may not behave correctly.",
                    "/tmp unmounted.");

        std::cout << "Unmounting proc in /proc..." << std::endl;
	unmount_drive("/proc",
                    "Failed to unmount /proc. The system may not behave correctly.",
                    "/proc unmounted.");
        
        std::cout << "Unmounting sysfs in /sys..." << std::endl;
        unmount_drive("/sys",
                    "Failed to mount /sys. The system may not behave correctly.",
                    "/sys mounted.");

	std::cout << "Unmounting devtmpfs in /dev..." << std::endl;
        unmount_drive("/dev",
                    "Failed to unmount /dev. The system may not behave correctly.",
                    "/dev unmounted.");
	
	return;

}

void remount_root_rw(unsigned long rootfs_mount_flags) {
	std::cout << "Remounting root as read-write." << std::endl;
	if (mount("", "/", "", rootfs_mount_flags, "") !=0 ) {
		util::panic("Failed to remount rootfs. Cannot proceed.");
	} else {
		util::ok("Rootfs has been remounted");
	}

	return;
}

void remount_root_ro() {
	std::cout << "Remounting root as read-only." << std::endl;
        sync();
        if (mount("", "/", "", MS_RDONLY | MS_REMOUNT, "") !=0 ) {
		util::panic("Failed to remount rootfs. Cannot proceed.");
	} else {
		util::ok("Rootfs has been remounted");
	}

	return;
}
void mount_root(const char* rootfs, const char* rootfs_type, unsigned long rootfs_mount_flags) {
	std::cout << "Mounting " << rootfs << " as a " << rootfs_type << " root device." << std::endl;
	if (mount(rootfs, "/new_root", rootfs_type, rootfs_mount_flags, "") !=0 ) {
		util::panic("Failed to mount rootfs on /new_root. Cannot proceed.");
	} else {
		util::ok("Rootfs has been mounted on /new_root");
	}

	return;
}

}
