// Microd, a simple futiristic init system for Linux
// Copyright (C) 2022-2022 Mutta Filippo
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include "rdinit.h"
#include "mount_special.h"
#include "util.h"
#include "postinit.h"
#include "rootutils.h"

const char* version = "0.2.0-alpha-rc1";
const char* author = "Mutta Filippo";
const char* date = "2022-2022";

util::arguments init_arguments;

enum runlevel {
	OFF,
	SINGLE,
	MULTI,
	MULTINET,
	MULTIP,
	FULL,
	REBOOT
} boot_runlevel;

void sig_handler(int signum);

void parse_arguments(int argc, char** argv);
void initrd_init();

int main(int argc, char** argv) {

	if (getpid() != 1) {
		std::cout << "Not running as PID 1." << std::endl;
		exit(0);
	}

	signal(SIGINT, sig_handler);
	signal(SIGHUP, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGKILL, sig_handler);
	signal(SIGTRAP, sig_handler);
	signal(SIGABRT, sig_handler);
	signal(SIGFPE, sig_handler);
	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);


	init_arguments.is_debug = false;
	init_arguments.is_in_root = false;
	init_arguments.rootdrv = "";
	init_arguments.rootfstype = "";
	boot_runlevel = OFF;

	parse_arguments(argc, argv);
		
	if (!init_arguments.is_in_root) {
		std::cout << "Hello, world!" << std::endl
			  << "Microd version " << version << ", Copyright (C) " << date << " " << author << std::endl
		          << "Microd comes with ABSOLUTELY NO WARRANTY." << std::endl
		          << "This is free software, and you are welcome to redistribute it." << std::endl
			  << "under certain conditions. Please consult the LICENSE file," << std::endl
		          << "located in the program's repository, for more information." << std::endl;

		initrd_init(init_arguments);
	}

	util::ok("Started root /init");

	mount_specialfs();

	root::remount_root_rw(init_arguments.rootdrv, init_arguments.rootfstype, MS_REMOUNT | MS_NOATIME);

	util::ok("Finished mounting");

	std::cout << "Hello, world!" << std::endl
	          << "Microd version " << version << ", Copyright (C) " << date << " " << author << std::endl
	          << "Microd comes with ABSOLUTELY NO WARRANTY." << std::endl
	          << "This is free software, and you are welcome to redistribute it." << std::endl
		  << "under certain conditions. Please consult the LICENSE file," << std::endl
	          << "located in the program's repository, for more information." << std::endl;

	switch (boot_runlevel) {
		case OFF:
			util::warning("Turning off...");
			util::change_state(util::sys_poweroff);
		case REBOOT:
			util::warning("Rebooting...");
			util::change_state(util::sys_halt);
			break;
		case SINGLE:
			util::warning("Starting runlevel 1");
			util::change_state(util::sys_runlevel_1);
		case MULTI:
			util::ok("Started runlevel 2");
		case MULTINET:
			util::ok("Started runlevel 3");
		case MULTIP:
			util::ok("Started runlevel 4");
		case FULL:
			util::ok("Started runlevel 5");
			util::change_state(util::sys_runlevel_5);
			break;
	}

	post_init(init_arguments.is_debug);

	util::panic("Something is wrong. We have passed the post_init() function.");

	util::change_state(util::sys_halt);
}

inline void parse_arguments(int argc, char** argv) {
	for (int i=0; i<argc; i++) {
		if(strcmp(argv[i], "in_root") == 0)
			init_arguments.is_in_root = true;
		else if(strcmp(argv[i], "init_debug") == 0)
			init_arguments.is_debug = true;
		else if(strcmp(argv[i], "0") == 0)
			boot_runlevel = OFF;
		else if(strcmp(argv[i], "1") == 0)
			boot_runlevel = SINGLE;
		else if(strcmp(argv[i], "2") == 0)
			boot_runlevel = MULTI;
		else if(strcmp(argv[i], "3") == 0)
			boot_runlevel = MULTINET;
		else if(strcmp(argv[i], "4") == 0)
			boot_runlevel = MULTIP;
		else if(strcmp(argv[i], "5") == 0)
			boot_runlevel = FULL;
		else if(strcmp(argv[i], "6") == 0)
			boot_runlevel = REBOOT;
	}
}

void sig_handler(int signum) {
	(void)signum;
	util::change_state(util::sys_halt);
}
