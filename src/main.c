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

const char* version = "0.1.0-alpha";

struct arguments {
	bool is_in_root;
	bool is_debug;
	char* rootdrv;
	char* rootfstype;
} init_arguments;

enum runlevel {
	OFF,
	SINGLE,
	MULTI,
	MULTINET,
	MULTIP,
	FULL,
	REBOOT
} curr_runlevel, boot_runlevel;

void sig_handler(int signum);

void parse_arguments(int argc, char** argv);
void initrd_init();
void root_init();

int main(int argc, char** argv) {

	if (getpid() != 1) {
		printf("Not running as PID 1.\n");
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
		initrd_init();
	}

	switch (boot_runlevel) {
		case OFF:
		case REBOOT:
			warning("Rebooting...");
			reboot();
			break;
		case SINGLE:
			warning("Starting runlevel 1");
			debug_shell();
			printf("Passing to runlevel 5...\n");
		case MULTI:
			ok("Started runlevel 2");
		case MULTINET:
			ok("Started runlevel 3");
		case MULTIP:
			ok("Started runlevel 4");
		case FULL:
			ok("Started runlevel 5");
			root_init();
			break;
	}

	post_init(init_arguments.is_debug);

	panic("Something is wrong. We have passed the post_init() function.");

	reboot();
}

inline void parse_arguments(int argc, char** argv) {
	for (int i=0; i<argc; i++) {
		if(strcmp(argv[i], "in_root") == 0)
			init_arguments.is_in_root = true;
		else if(strcmp(argv[i], "init_debug") == 0)
			init_arguments.is_debug = true;
		else if(strcmp(argv[i], "0") == 0)
			boot_runlevel = 0;
		else if(strcmp(argv[i], "1") == 0)
			boot_runlevel = 1;
		else if(strcmp(argv[i], "2") == 0)
			boot_runlevel = 2;
		else if(strcmp(argv[i], "3") == 0)
			boot_runlevel = 3;
		else if(strcmp(argv[i], "4") == 0)
			boot_runlevel = 4;
		else if(strcmp(argv[i], "5") == 0)
			boot_runlevel = 5;
		else if(strcmp(argv[i], "6") == 0)
			boot_runlevel = 6;
	}
}

inline void initrd_init() {
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

inline void root_init() {
	ok("Started root /init");
	printf("Welcome to Linux!\n\a"
	       "Hello, World!\n"
	       "Mutta-Init v%s\n", version);

	mount_specialfs();

	remount_root_rw(init_arguments.rootdrv, init_arguments.rootfstype, MS_REMOUNT | MS_NOATIME);

	ok("Finished mounting");

	startup_scripts();

	ok("Finished running startups scripts");
	
	launch_programs();

	ok("Finished launching programs");

	ok("System is booted!!!");
}

void sig_handler(int signum) {
	(void)signum;
	reboot();
}
