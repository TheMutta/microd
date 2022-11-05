#include "postinit.h"

void post_init(bool debug) {
	std::cout << "Init has entered quiet mode." << std::endl
		  << "Unless debug mode is active, only critical messages will be sent." << std::endl;
	
	if (debug)
		util::ok("Housekeeping started.");

	std::cout << "Currently running units: " << unit::managed_units.size() << std::endl;

	while (true) {
		// Tick tock
		sleep(1);
	}
}
