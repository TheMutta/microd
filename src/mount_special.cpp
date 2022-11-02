#include "mount_special.h"

void mount_specialfs() {
	printf("Mounting filesystems...\n");

	printf("Mounting devtmpfs in /dev...\n");
	if (mount("none", "/dev", "devtmpfs", MS_NOSUID, "") !=0 ) {
		warning("Failed to mount /dev. The system may not behave correctly");
	} else {
		ok("/dev mounted");
	}

	printf("Mounting sysfs in /sys...\n");
	if (mount("none", "/sys", "sysfs", MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_NOSUID | MS_RELATIME, "") !=0) {
		warning("Failed to mount /sys. The system may not behave correctly.");
	} else {
		ok("/sys mounted");
	}

	printf("Mounting proc in /proc...\n");
	if (mount("none", "/proc", "proc", MS_NOATIME | MS_NODEV | MS_NOEXEC | MS_NOSUID | MS_RELATIME, "") !=0 ) {
		warning("Failed to mount /proc. The system may not behave correctly.");
	} else {
		ok("/proc mounted");
	}

	printf("Mounting tmpfs in /tmp...\n");
	if (mount("none", "/tmp", "tmpfs", MS_NOATIME | MS_NODEV | MS_NOSUID, "") !=0 ) {
		warning("Failed to mount /tmp. The system may not behave correctly.");
	} else {
		ok("/tmp mounted");
	}

	return;
}


