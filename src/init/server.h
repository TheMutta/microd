#pragma once
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

namespace server {
        extern const char* socket_name;
        
        void init_socket();
        void run_socket();
        void close_socket();
}
