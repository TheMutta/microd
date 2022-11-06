#include "unit.h"

namespace unit {


std::vector<Unit> managed_units;

enum valid_actions { action_start,
		     action_executable,
		     action_message,
		     action_requires,
		     action_restart
		   };

std::map<std::string, valid_actions> mapped_actions;


void init() {
	mapped_actions["start"] = action_start;
	mapped_actions["exec"] = action_executable;
	mapped_actions["mesg"] = action_message;
	mapped_actions["before"] = action_requires;
	mapped_actions["restart"] = action_restart;
}


int run_unit(std::string unit_file, util::runlevel level) {
	std::ifstream file;
	std::string line,
		    action,
		    runlevel,
		    executable_cmd,
		    message_text,
		    required_unit,
		    restart;

	file.open(unit_file.c_str());

	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::stringstream curr_line(line);
			std::getline(curr_line, action, ' ' );
			switch (mapped_actions[action]) {
				case action_start:
					std::getline(curr_line, runlevel);
					break;
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

		if (runlevel == "all")
			void;
			// go on
		else {
			if (runlevel == "2" && level >= util::MULTI)
				void;
				// go on
			else if (runlevel == "3" && level >= util::MULTINET)
				void;
				// go on
			else if (runlevel == "4" && level >= util::MULTIP)
				void;
				// go on
			else if (runlevel == "5" && level >= util::FULL)
				void;
				// go on
			else {
				std::cout << "Unit " << unit_file << " runs in runlevel " << runlevel << " or higher. Not starting it.\n";
				return -1;
			}
		}
		
		std::cout << unit_file << " says: " << message_text << std::endl;

		std::stringstream parse_exec(executable_cmd);
		std::string value;
		std::vector<std::string> executable;

		while (std::getline(parse_exec, value, ' ')) {
			executable.push_back(value);
		}

		for (int i = 0; i < managed_units.size(); i++) {
			if (unit_file == managed_units[i].file) {
				// Unit already exists, not restarting
				return 1;
			}
		}

		pid_t daemon = fork();
		if (daemon == 0) {
			util::exec(executable);
			exit(1);
		}

		managed_units.push_back(Unit());
		managed_units[managed_units.size() - 1].file = unit_file;
		managed_units[managed_units.size() - 1].pid = daemon;

		if (restart == "always") {
			managed_units[managed_units.size() - 1].restart = true;
			managed_units[managed_units.size() - 1].restart_if_stopped = true;
		} else if (restart == "unless stopped") {
			managed_units[managed_units.size() - 1].restart = true;
		} else {
			// Is either never or not valid
		}

		return 0;
	} else {
		return -1;
	}
}
}
