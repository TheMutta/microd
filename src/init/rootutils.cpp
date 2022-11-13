#include "rootutils.h"

namespace root {

void launch_programs(state::runlevel level) {
	std::cout << "Launching programs..." << std::endl;

	unit::init();

        std::ifstream inittab_file;
        inittab_file.open("/etc/inittab");

        /*tinydir_dir dir;
	tinydir_open(&dir, "/etc/init");*/


        if(inittab_file.is_open()) {
                std::string line, runlevel, instruction, file_name;
                int launch_runlevel = 1;
                while(std::getline(inittab_file, line)) {
              		std::stringstream curr_line(line);
                	std::getline(curr_line, instruction, ' ' );
                        if (instruction == "runlevel") {
                	        std::getline(curr_line, runlevel, ' ' );
                                launch_runlevel = atoi(runlevel.c_str());
                        } else {
                                file_name = instruction;
                                file_name = "/etc/init/" + file_name + ".unit";
				std::cout << "Runnning " << file_name << " as an unit." << std::endl;
                                unit::run_unit(file_name, level, launch_runlevel);
                        }
                        
                }
        }

        inittab_file.close();
/*
	while (dir.has_next) {
		tinydir_file file;
        	tinydir_readfile(&dir, &file);


		if (!file.is_dir) {
			char file_name[512] = {0};
			strcat(file_name, "/etc/autostart/");
			strcat(file_name, file.name);
			if (strcmp(file.extension, "unit") == 0) {
				unit::run_unit(file_name, level);
			} else if (strcmp(file.extension, "disabled") == 0) {
				std::cout << "Ignoring " << file_name << "..." << std::endl;
				// It's disabled, ignore it
			} else if (strcmp(file.extension, "sh") == 0) {
				// Executing it as a shell script
				std::cout << "Executing shell script " << file_name << "..." << std::endl;
				pid_t pid = fork();
				if (pid == 0) {
					execl(file_name, file.name, (char*)NULL);
					perror("execl");
					exit(1);
				} 
			} else {
				// Executing it as a normal executable
				std::cout << "Executing file " << file_name << "..." << std::endl;
				pid_t pid = fork();
				if (pid == 0) {
					execl(file_name, file.name, (char*)NULL);
					perror("execl");
					exit(1);
				}
			}
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);	
*/
}

}
