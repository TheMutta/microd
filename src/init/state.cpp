#include "state.h"
#include "util.h"
#include "rootutils.h"
#include "mount.h"

namespace state {

// The curren runlevel we're in 
runlevel curr_runlevel;

/*
 * change_runlevel:
 *  This function is used to change runlevel to the specified runlevel. It will kill all
 *  the units that have gone out of scope and it will eventually, in case of an OFF or REBOOT,
 *  unmount mounted drives
 */
void change_runlevel(runlevel level) {
	curr_runlevel = level;
        unit::kill_units(curr_runlevel);
	std::cout << " * Switched to runlevel " << curr_runlevel << std::endl;
	switch(curr_runlevel) {
		case OFF:
                        sync();
                        mounting::remount_root_ro();
                        mounting::unmount_fstab();
			break;
		case SINGLE:
                        std::cout << " * Entering mainenance mode." << std::endl;
                        //TODO
			break;
		case MULTI:
			root::launch_programs(curr_runlevel);
			break;
		case MULTINET:
			root::launch_programs(curr_runlevel);
			break;
		case MULTIP:
			root::launch_programs(curr_runlevel);
			break;
		case FULL:
			root::launch_programs(curr_runlevel);
			break;
		case REBOOT:
                        sync();
                        mounting::remount_root_ro();
                        mounting::unmount_fstab();
			break;

	}
}

/*
 * change_runlevel:
 *  This function is used to change the current state of init. It is the main component
 *  of the state machine.
 */
void change_state(change_action action) {
	sync();
	
	switch (action) {
		default:
			break;
		case sys_reboot:
			change_runlevel(REBOOT);
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART, 0);
			break;
		case sys_poweroff:
			change_runlevel(OFF);
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_POWER_OFF, 0);
			break;
		case sys_halt:
			change_runlevel(OFF);
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_HALT, 0);
			break;
		case sys_suspend:
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_SW_SUSPEND, 0);
			break;
		case sys_kexec:
			change_runlevel(OFF);
			syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_KEXEC, 0);
			break;
		case sys_runlevel_1:
			change_runlevel(SINGLE);
			break;
		case sys_runlevel_2:
			change_runlevel(MULTI);
			break;
		case sys_runlevel_3:
			change_runlevel(MULTINET);
			break;
		case sys_runlevel_4:
			change_runlevel(MULTIP);
			break;
		case sys_runlevel_5:
			change_runlevel(FULL);
			break;
	}
}



}
