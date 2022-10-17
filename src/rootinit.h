#pragma once
#include <sys/mount.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "util.h"

void mount_drive();
void startup_scripts();
void launch_daemons(); 
