#include "postinit.h"

inline void post_init(bool debug) {
	printf("Init has entered quiet mode.\nUnless debug mode is active, only critical messages will be sent.\n");
	if (debug)
		ok("Housekeeping started.");
	while (true) {
		sleep(1);
	}
}
