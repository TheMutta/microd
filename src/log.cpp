#include "log.h"

inline void setup_log() {
	printf("Setting up a log in /var/log/init.log...");
	// Do stuff...
	ok("Done setting up a log.");

}

void print_to_log(std::string message) {
	// Do stuff...
}

inline void close_log() {
	printf("Shutting down the log...");
	// Do stuff...
	ok ("Log closed.");
}
