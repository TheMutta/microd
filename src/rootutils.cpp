#include "rootutils.h"

namespace root {

void startup_scripts() {
	std::cout << "Running startup scripts..." << std::endl;

	pid_t pid=fork();

	if (pid==0) {
		static char *argv[]={"busybox", "sh", "/etc/inittab", NULL};
		static char *env[]={"PATH=/bin:/sbin", NULL};
		execve("/bin/busybox", argv, env);
		perror("execve");
		exit(1);
	} else {
		waitpid(pid,0,0);
	}

	return;
}


void launch_programs(state::runlevel level) {
	std::cout << "Launching programs..." << std::endl;

	unit::init();

	tinydir_dir dir;
	tinydir_open(&dir, "/etc/autostart");

	while (dir.has_next) {
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (!file.is_dir) {
			char file_name[512] = {0};
			strcat(file_name, "/etc/autostart/");
			strcat(file_name, file.name);
			if (strcmp(file.extension, "unit") == 0) {
				std::cout << "Runnning " << file_name << " as an unit." << std::endl;
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
}

}
