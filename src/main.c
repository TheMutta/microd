#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include "rdinit.h"
#include "rootinit.h"
#include "mount_special.h"
#include "util.h"
#include "postinit.h"

const char* version = "0.0.0.1-alpha1-devel";

struct arguments {
	bool is_in_root;
	bool is_debug;
	char* rootdrv;
	char* rootfstype;
};

void sig_handler(int signum);

int main(int argc, char** argv) {
	if (getpid() == 1) {
		struct arguments init_arguments;
		init_arguments.is_debug = false;
		init_arguments.is_in_root = false;
		init_arguments.rootdrv = "";
		init_arguments.rootfstype = "";

		signal(SIGINT, sig_handler);
		signal(SIGHUP, sig_handler);
		signal(SIGQUIT, sig_handler);
		signal(SIGKILL, sig_handler);
		signal(SIGTRAP, sig_handler);
		signal(SIGABRT, sig_handler);
		signal(SIGFPE, sig_handler);
		signal(SIGUSR1, sig_handler);
		signal(SIGUSR2, sig_handler);


		for (int i=0; i<argc; i++) {
			if(strcmp(argv[i], "in_root") == 0)
				init_arguments.is_in_root = true;
			else if(strcmp(argv[i], "init_debug") == 0)
				init_arguments.is_debug = true;
		}


		if (!init_arguments.is_in_root) {
			ok("Started initrd /init");
			printf("Hello, world!\n"
			       "Mutta-Init v%s\n", version);

			mount_specialfs();

			ok("Finished mounting filesystems.");

			if (init_arguments.is_debug) {
				debug_shell();
			}
		
			mount_root(init_arguments.rootdrv, init_arguments.rootfstype, MS_RDONLY | MS_NOATIME);
			switch_root();
		}
	
		ok("Started root /init");
		printf("Welcome to Linux!\n\a"
		       "Hello, World!\n"
		       "Mutta-Init v%s\n", version);

		if(init_arguments.is_debug)
			warning("Debug mode active");

		mount_specialfs();

		remount_root_rw(init_arguments.rootdrv, init_arguments.rootfstype, MS_REMOUNT | MS_NOATIME);

		ok("Finished mounting");

		startup_scripts();

		ok("Finished running startups scripts");
	
		launch_programs();

		ok("Finished launching programs");

		ok("System is booted!!!");

		post_init(init_arguments.is_debug);

		panic("Something is wrong. We have passed the post_init() function.");
	} else {
		printf("Not running as PID 1, stopping");
		exit(1);
	}

	reboot();
}

void sig_handler(int signum){
	(void)signum;
	reboot();
}
