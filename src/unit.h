#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <unistd.h>
#include <cstring>

#include "util.h"

namespace unit {

struct Unit {
	std::string file;
	pid_t pid;
	int status;
};

extern std::vector<Unit> managed_units;

int run_unit(std::string unit_file);
void init();

}
