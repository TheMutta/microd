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

int run_unit(char *unit_file);
void init();

}
