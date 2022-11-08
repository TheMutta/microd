#pragma once
#include <sys/mount.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>

#include "util.h"
#include "unit.h"
#include "tinydir.h"
#include "mount.h"

namespace root {
	void startup_scripts();
	void launch_programs(util::runlevel level); 
}
