#include "parsefile.h"

namespace file {
unit::Unit parse_unit_file(std::string file_path) {
        unit::Unit new_unit;

        std::vector<std::string> actions_name;
        actions_name.push_back("exec");
        actions_name.push_back("mesg");
        actions_name.push_back("before");
        actions_name.push_back("restart");

        std::map<std::string,std::string> parsed_file = parse_file(file_path, actions_name);
        
        new_unit.file = file_path;
        new_unit.message = parsed_file["mesg"];
        new_unit.exec = parsed_file["exec"];

	if (parsed_file["restart"] == "always") {
      		new_unit.restart = true;
       	} else if (parsed_file["restart"] == "nostop") {
        	new_unit.restart = false;
	        new_unit.restart_unless_stopped = true;
       	} else {
                new_unit.restart = false;
        	new_unit.restart_unless_stopped = false;
       	}

        return new_unit;
}

std::map<std::string,std::string> parse_file(std::string file_path, std::vector<std::string> actions_name) {
        std::ifstream file;
        std::string line, action;
        
        std::map<std::string,std::string> parsed_file;

	file.open(file_path);

	if (file.is_open()) {
		while (std::getline(file, line)) {
                        std::vector<std::string>::iterator it;
			std::stringstream curr_line(line);
			std::getline(curr_line, action, ' ' );
                        it = find(actions_name.begin(), actions_name.end(), action);
                        if (it != actions_name.end()) {
        			std::getline(curr_line, action);
                                parsed_file[actions_name[it - actions_name.begin()]] = action;
                        }
		}


		file.close();
	}

        return parsed_file;
}
}
