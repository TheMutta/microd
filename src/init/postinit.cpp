#include "postinit.h"

#include "unit.h"
#include "util.h"
#include "server.h"

static bool is_debug;

void post_init(bool debug) {
        is_debug = debug;

	std::cout << "Init has entered quiet mode." << std::endl
		  << "Unless debug mode is active, only critical messages will be sent." << std::endl;

	std::cout << "Currently running units: " << unit::managed_units.size() << std::endl;

        server::init_socket();

        while (true);
        
        server::close_socket();
}
