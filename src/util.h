#pragma once
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <linux/reboot.h>
#include <unistd.h>

void ok(std::string message);
void panic(std::string message);
void warning(std::string message);
void debug_shell();
void reboot();
