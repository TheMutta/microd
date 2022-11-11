#pragma once
#include <unistd.h>

namespace state {
        enum runlevel {
		OFF,
		SINGLE,
		MULTI,
		MULTINET,
		MULTIP,
		FULL,
		REBOOT
	};

	enum change_action { sys_reboot,
			     sys_poweroff,
			     sys_halt,
			     sys_suspend,
			     sys_kexec,
			     sys_runlevel_1,
			     sys_runlevel_2,
			     sys_runlevel_3,
			     sys_runlevel_4,
			     sys_runlevel_5,
	};

	extern runlevel curr_runlevel;

        void change_runlevel(runlevel level);
	void change_state(change_action action);

}
