#pragma once
#include <sys/mount.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "util.h"
#include "unit.h"
#include "tinydir.h"
#include "mount.h"

namespace root {
	void startup_scripts();
	void launch_programs(state::runlevel level); 
}
