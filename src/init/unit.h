#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <unistd.h>
#include <cstring>
#include <variant>

#include "util.h"
#include "state.h"

namespace unit {
        struct Unit {
	        std::string file = "";
        	state::runlevel runlevel = 0;

                bool active = false;

        	pid_t pid = 0;
                int status = 0;

                bool restart = false;
	        bool restart_unless_stopped = false;

        	bool is_stopped = false;
        };

        extern std::vector<Unit> managed_units;

        int run_unit(std::string unit_file,
                     state::runlevel level,
                     state::runlevel launch_runlevel);
        void kill_units(state::runlevel level);
        void init();
}
