#include "rootutils.h"

namespace root {

void launch_programs(state::runlevel level) {
	std::cout << "Launching programs..." << std::endl;

	unit::init();

        std::ifstream inittab_file;
        inittab_file.open("/etc/inittab");

        if(inittab_file.is_open()) {
                std::string line, runlevel, instruction, file_name;
                int launch_runlevel = 1;
                while(std::getline(inittab_file, line)) {
              		std::stringstream curr_line(line);
                	std::getline(curr_line, instruction, ' ' );
                        if (instruction == "runlevel") {
                	        std::getline(curr_line, runlevel, ' ' );
                                launch_runlevel = atoi(runlevel.c_str());
                                std::cout << "Starting units for runlevel " << launch_runlevel << std::endl;
                        } else {
                                file_name = instruction;
                                file_name = "/etc/init/" + file_name + ".unit";
				std::cout << "Runnning " << file_name << " as an unit." << std::endl;
                                unit::run_unit(file_name, level, launch_runlevel);
                        }
                        
                }
        }

        inittab_file.close();
}

}
