#pragma once
#include <unistd.h>

#include "util.h"

void setup_log();
void print_to_log(char* message);
void close_log();
