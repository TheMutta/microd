#include "mount.h"

namespace mounting {

void mount_fstab() {
        std::cout << " * Loading fstab..." << std::endl;
        if(!std::filesystem::exists("/etc/fstab")) {
                util::warning();
                std::cout << " -> Fstab does not exist, loading default filesystems..." << std::endl;
                mount_specialfs();
        }
        FILE *fstab = setmntent("/etc/fstab", "r");
        struct mntent *mount_fs;
        while ((mount_fs = getmntent(fstab))) {
	        std::cout << " -> Mounting " << mount_fs->mnt_fsname << " " << mount_fs->mnt_type << " in " << mount_fs->mnt_dir << "..." << std::endl;
                std::string command;
                command = "mount ";
                command = command.append(mount_fs->mnt_dir);
                system(command.c_str());
        }

        endmntent(fstab);
}

void unmount_fstab() {
        FILE *fstab = setmntent("/etc/fstab", "r");
        struct mntent *mount_fs;
        while ((mount_fs = getmntent(fstab))) {
                if(strcmp(mount_fs->mnt_type, "devtmpfs") != 0 &&
                     strcmp(mount_fs->mnt_type, "devpts") != 0 &&
                     strcmp(mount_fs->mnt_type, "sysfs") != 0 &&
                     strcmp(mount_fs->mnt_type, "proc") != 0)
                        unmount_drive(mount_fs->mnt_dir);
        }

        endmntent(fstab);
}


void mount_drive(const char* drv,
                 const char* destdir,
                 const char* fstype,
                 unsigned long flags) {
	std::cout << " * Mounting " << drv << " " << fstype << " in " << destdir << "..." << std::endl;

        int errnum = mount(drv, destdir, fstype, flags, "");
        if (errnum !=0 ) {
		util::warning();
                std:: cout << " -> Failed to mount. Errorcode: " << ((errno == EACCES) ? "EACCESS" : "unknown") << std::endl;
	} else {
		util::ok();
	}

	return;
}

void unmount_drive(const char* destdir) {
	std::cout << " * Unmounting filesystem in " << destdir << "..." << std::endl;

        if (umount(destdir) != 0) {
	       	util::warning();
	} else {
		util::ok();
	}

	return;
}


void mount_specialfs() {
        mount_drive("none", "/dev", "devtmpfs", MS_NOSUID);

        mount_drive("none", "/sys", "sysfs",
                    MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_NOSUID | MS_RELATIME);

	mount_drive("none", "/proc", "proc",
                    MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_NOSUID | MS_RELATIME);

	mount_drive("none", "/tmp", "tmpfs",
                    MS_NOATIME | MS_NODEV | MS_NOSUID);

	return;
}

void unmount_specialfs() {
        std::cout << " * Unmounting filesystems..." << std::endl;
	
	unmount_drive("/tmp");
	unmount_drive("/proc");
        unmount_drive("/sys");
        unmount_drive("/dev");
	
	return;

}

void remount_root_rw(unsigned long rootfs_mount_flags) {
	std::cout << " * Remounting root as read-write..." << std::endl;
	if (mount("", "/", "", rootfs_mount_flags, "") !=0 ) {
		util::panic();
	} else {
		util::ok();
	}

	return;
}

void remount_root_ro() {
	std::cout << " * Remounting root as read-only..." << std::endl;
        sync();
        if (mount("", "/", "", MS_RDONLY | MS_REMOUNT, "") !=0 ) {
		util::panic();
	} else {
		util::ok();
	}

	return;
}
void mount_root(const char* rootfs, const char* rootfs_type, unsigned long rootfs_mount_flags) {
	std::cout << " * Mounting " << rootfs << " as a " << rootfs_type << " root device..." << std::endl;
	if (mount(rootfs, "/new_root", rootfs_type, rootfs_mount_flags, "") !=0 ) {
		util::panic();
	} else {
		util::ok();
	}

	return;
}

}
