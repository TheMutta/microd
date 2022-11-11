#pragma once
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "unit.h"
#include "server.h"
#include "util.h"

void post_init(bool debug);
