#include "postinit.h"

void post_init(bool debug) {
	std::cout << "Init has entered quiet mode." << std::endl
		  << "Unless debug mode is active, only critical messages will be sent." << std::endl;


	if (debug)
		util::ok("Housekeeping started.");

	if (getpid() == 1) {
		unit::init();
		std::cout << "Currently running units: " << unit::managed_units.size() << std::endl;

		while (true) {
			for (unsigned long int i = 0; i < unit::managed_units.size(); i++) {
				if(waitpid(unit::managed_units[i].pid, &unit::managed_units[i].status, WNOHANG) != 0) {
					std::cout << "Unit " << unit::managed_units[i].file << " PID " <<  unit::managed_units[i].pid << " has status " << unit::managed_units[i].status << std::endl;
					if (WIFEXITED(unit::managed_units[i].status)) {
						std::string unit_name;
						if (WEXITSTATUS(unit::managed_units[i].status) == 0) {
							if (unit::managed_units[i].restart && unit::managed_units[i].restart_if_stopped) {
								std::cout << "Process " << unit::managed_units[i].file
									  << " exited successfully. Restarting..." << std::endl;
								unit_name = unit::managed_units[i].file;
								unit::managed_units.erase(unit::managed_units.begin() + i);
								if (debug)
									std::cout << "Removed unit number " << i << std::endl;
	
								unit::run_unit(unit_name);
								if (debug)
									std::cout << "Launched unit successfully!" << std::endl;
							} else {
								std::cout << "Process " << unit::managed_units[i].file
									  << " exited successfully." << std::endl;
								unit::managed_units.erase(unit::managed_units.begin() + i);

							}
						} else {
							std::cout << "Process " << unit::managed_units[i].file << " exited unsuccessfully. Not restarting..." << std::endl;
							unit::managed_units.erase(unit::managed_units.begin() + i);
							break;
						}

						break;
					}

				}
			}

			sleep(1);
		}

	} else {
		// Possible userspace ?
	}
}
