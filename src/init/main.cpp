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
#include "postinit.h"
#include "mount.h"
#include "util.h"
#include "rootutils.h"
#include "server.h"
#include "unit.h"
#include "../config.h"

util::arguments init_arguments;

state::runlevel boot_runlevel;

void sig_handler(int sig, siginfo_t *info, void *ucontext);

void parse_arguments(int argc, char** argv);

int main(int argc, char** argv) {
        // Checking if we're not in PID 1
	if (getpid() != 1) {
		std::cout << "Not running as PID 1." << std::endl;
		exit(1);
	}

        // Setting up signal handling
        struct sigaction new_action;
        new_action.sa_sigaction = sig_handler;
        sigemptyset(&new_action.sa_mask);
        new_action.sa_flags = SA_SIGINFO;

        sigaction(SIGTERM, &new_action, NULL);
        sigaction(SIGUSR1, &new_action, NULL);
        sigaction(SIGUSR2, &new_action, NULL);
        sigaction(SIGCHLD, &new_action, NULL);
        sigaction(SIGCONT, &new_action, NULL);

        // Temporary setting of init_arguments until a proper parsing system is put in place
	init_arguments.is_debug = false;
	init_arguments.is_in_root = false;
	init_arguments.rootdrv = "";
	init_arguments.rootfstype = "";
	boot_runlevel = state::FULL;

        // Parsing arguments (TODO: to be overhauled)
	parse_arguments(argc, argv);
	
        // Initrd phase:
        //  This phase is executed when init is started in an initramfs
	if (!init_arguments.is_in_root) {
		std::cout << "Hello, world!" << std::endl
			  << "Microd version " << version << ", Copyright (C) " << date << " " << author << std::endl
		          << "Microd comes with ABSOLUTELY NO WARRANTY." << std::endl
		          << "This is free software, and you are welcome to redistribute it." << std::endl
			  << "under certain conditions. Please consult the LICENSE file," << std::endl
		          << "located in the program's repository, for more information." << std::endl;

		initrd_init(init_arguments);
	}

        std::cout << " * Starting root init..." << std::endl;
	util::ok();

        std::cout << " * Mounting drives..." << std::endl;

 //       mounting::mount_specialfs();

        mounting::mount_drive("none", "/dev", "devtmpfs", MS_NOSUID);

        mounting::mount_fstab();

//	mounting::remount_root_rw(MS_REMOUNT | MS_NOATIME);

	std::cout << "Hello, world!" << std::endl
	          << "Microd version " << version << ", Copyright (C) " << date << " " << author << std::endl
	          << "Microd comes with ABSOLUTELY NO WARRANTY." << std::endl
	          << "This is free software, and you are welcome to redistribute it." << std::endl
		  << "under certain conditions. Please consult the LICENSE file," << std::endl
	          << "located in the program's repository, for more information." << std::endl;

	switch(boot_runlevel) {
		case state::OFF:
			state::change_state(state::sys_halt);
			break;
		case state::SINGLE:
			state::change_state(state::sys_runlevel_1);
			break;
		case state::MULTI:
			state::change_state(state::sys_runlevel_2);
			break;
		case state::MULTINET:		
			state::change_state(state::sys_runlevel_3);
			break;
		case state::MULTIP:
			state::change_state(state::sys_runlevel_4);
			break;
		case state::FULL:
			state::change_state(state::sys_runlevel_5);
			break;
		case state::REBOOT:
			state::change_state(state::sys_reboot);
			break;
	}

	post_init(init_arguments.is_debug);

        std::cout << " * Something is wrong. We have passed the post_init() function." << std::endl;
	util::panic();

	state::change_state(state::sys_halt);
}

inline void parse_arguments(int argc, char** argv) {
	for (int i=0; i<argc; i++) {
		if(strcmp(argv[i], "in_root") == 0)
			init_arguments.is_in_root = true;
		else if(strcmp(argv[i], "init_debug") == 0)
			init_arguments.is_debug = true;
		else if(strcmp(argv[i], "0") == 0)
			boot_runlevel = state::OFF;
		else if(strcmp(argv[i], "1") == 0)
			boot_runlevel = state::SINGLE;
		else if(strcmp(argv[i], "2") == 0)
			boot_runlevel = state::MULTI;
		else if(strcmp(argv[i], "3") == 0)
			boot_runlevel = state::MULTINET;
		else if(strcmp(argv[i], "4") == 0)
			boot_runlevel = state::MULTIP;
		else if(strcmp(argv[i], "5") == 0)
			boot_runlevel = state::FULL;
		else if(strcmp(argv[i], "6") == 0)
			boot_runlevel = state::REBOOT;
	}
}

void sig_handler(int sig, siginfo_t *info, void *ucontext) {
	switch(sig) {
		case SIGTERM:
			state::change_state(state::sys_reboot);
			break;
		case SIGUSR1:
			state::change_state(state::sys_halt);
			break;
		case SIGUSR2:
			state::change_state(state::sys_poweroff);
			break;
                case SIGCONT:
                        server::run_socket();
                        break;
                case SIGCHLD:
                        for(int i = 0; i < unit::managed_units.size(); i++)  {
                                if(unit::managed_units[i].pid == info->si_pid) {
                                        if(state::curr_runlevel != state::OFF &&
                                           state::curr_runlevel != state::REBOOT &&
                                           info->si_errno == 0) {
                                                std::string unit_name;
                                                unit_name = unit::managed_units[i].file;
        	        			unit::managed_units.erase(unit::managed_units.begin() + i);

                                                if (unit::managed_units[i].restart ||
                                                    (unit::managed_units[i].restart_unless_stopped &&
                                                     !unit::managed_units[i].is_stopped)) {
        					         unit::run_unit(unit_name, state::curr_runlevel, unit::managed_units[i].runlevel);
                                                }
                                        } else {
                                                std::cout << "Unit " << info->si_pid << " failed." << std::endl;
                                                unit::managed_units.erase(unit::managed_units.begin() + i);
                                        }
                                }
                        }
                        break;
		default:
			// Signal not recognized
			std::cout << "Recieved unrecognized signal: " << sig << std::endl;
			break;
        }
}
