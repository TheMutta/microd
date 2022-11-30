#pragma once
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include "unit.h"

namespace file {
        unit::Unit parse_unit_file(std::string file_path);
        std::map<std::string,std::string> parse_file(std::string file_path, std::vector<std::string> actions_name);
}
