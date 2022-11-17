#pragma once
#include <unistd.h>
#include <string>
#include <iostream>

#include "util.h"

void setup_log();
void print_to_log(std::string message);
void close_log();
