#include "log.h"

inline void setup_log() {
        std::cout << " * Setting up a log in /var/log/init.log..." << std::endl;
	// Do stuff...
	util::ok();

}

void print_to_log(std::string message) {
	// Do stuff...
}

inline void close_log() {
        std:: cout << " * Shutting down the log..." << std::endl;
	// Do stuff...
	util::ok ();
}
