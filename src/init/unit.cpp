#include "unit.h"

#include "parsefile.h"

namespace unit {

// Vector that contains all the units currently being managed by init
std::vector<Unit> managed_units;

// This are the valid actions that an unit file can contain
// the map<std::string, enum> is used so to remove the need of
// an if/else if/else statement chain and to allow for easy expandability
/*enum valid_actions {
        action_executable,
	action_message,
	action_requires,
	action_restart
};*/

/*std::map<std::string, valid_actions> mapped_actions;*/

/*
 * init:
 *  This function maps the right values to the correct actions
 *//*
void init() {
	mapped_actions["exec"] = action_executable;
	mapped_actions["mesg"] = action_message;
	mapped_actions["before"] = action_requires;
	mapped_actions["restart"] = action_restart;
}*/


int run_unit(std::string unit_file, state::runlevel level, state::runlevel launch_runlevel) {
	/*std::ifstream file;
	std::string line,
		    action,
		    executable_cmd,
		    message_text,
		    required_unit,
		    restart;

	file.open(unit_file);

	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::stringstream curr_line(line);
			std::getline(curr_line, action, ' ' );
			switch (mapped_actions[action]) {
				case action_executable:
					std::getline(curr_line, executable_cmd);
					break;
				case action_message:
					std::getline(curr_line, message_text);
					break;
				case action_requires:
					std::getline(curr_line, required_unit);
					break;
				case action_restart:
					std::getline(curr_line, restart);
					break;
				default:
					std::cout << "Action " << action << " not valid" << std::endl;
					break;
			}
		}


		file.close();
        }*/
        
        Unit unit = file::parse_unit_file(unit_file);

        if (launch_runlevel > level || launch_runlevel == state::OFF) {
	        std::cout << " -> Unit " << unit_file << " runs in runlevel " << launch_runlevel << " or higher. Not starting it." << std::endl;
		return -1;
        }
	std::cout << " -> " << unit_file << " says: " << unit.message << std::endl;

	std::stringstream parse_exec(unit.exec);
	std::string value;
	std::vector<std::string> executable;

	while (std::getline(parse_exec, value, ' ')) {
		executable.push_back(value);
	}

        bool already_exists = false;


	for (int i = 0; i < managed_units.size(); i++) {
		if (unit_file == managed_units[i].file) {
			// Unit already exists, restarting it
                        already_exists = true;
                        if(managed_units[i].active == true) {
                                std::cout << " ->> Already active" << std::endl;
                                return -1;
                        }
		}
	}

	pid_t daemon = fork();
	if (daemon == 0) {
		util::exec(executable);
		exit(1);
	}

        if (!already_exists) {
                unit.file = unit_file;
                unit.active = true;
                unit.pid = daemon;
                unit.runlevel = launch_runlevel;

	        /*if (restart == "always") {
        		unit.restart = true;
	       	} else if (restart == "nostop") {
	        	unit.restart = false;
		        unit.restart_unless_stopped = true;
        	} else {
                        unit.restart = false;
	        	unit.restart_unless_stopped = false;
        	}*/

	       	managed_units.push_back(unit);
        } else {
                for (int i = 0; i < managed_units.size(); i++) {
		      	if (unit_file == managed_units[i].file) {
                                managed_units[i].active = true;
                                managed_units[i].pid = daemon;
                        }
		}
	}

        return 0;
}

void kill_units(state::runlevel level) {
	for (unsigned long int i = 0; i < unit::managed_units.size(); i++) {
		if((level == state::OFF || level == state::REBOOT || unit::managed_units[i].runlevel > level ) && unit::managed_units[i].pid != -1) {
			std::cout << " -> Sending SIGTERM to " << unit::managed_units[i].file << std::endl;
			kill(unit::managed_units[i].pid, SIGTERM);
		}
	}

	for (unsigned long int i = 0; i < unit::managed_units.size(); i++) {
		if((level == state::OFF || level == state::REBOOT || unit::managed_units[i].runlevel > level ) && unit::managed_units[i].pid != -1) {
			std::cout << " -> Sending SIGKILL to " << unit::managed_units[i].file << std::endl;
			kill(unit::managed_units[i].pid, SIGKILL);
		}
	}
}


}
