#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <linux/reboot.h>
#include <unistd.h>

void ok(char* message);
void panic(char* message);
void warning(char* message);
void debug_shell();
void reboot();
