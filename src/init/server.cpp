#include "server.h"

#include "state.h"
#include "unit.h"

namespace server {

const char* socket_name = "/var/run/init.socket";
const unsigned short socket_buffer_size = 4096;
struct sockaddr_un name;
int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
int data_socket;
char buffer[socket_buffer_size];
int ret;
int down_flag = 0;

void init_socket() {
        if (data_socket == -1) {
                perror("socket");
        }

        /*
            * For portability clear the whole structure, since some
            * implementations have additional (nonstandard) fields in
            * the structure.
            */

        memset(&name, 0, sizeof(name));

        name.sun_family = AF_UNIX;
        strncpy(name.sun_path, socket_name, sizeof(name.sun_path) - 1);

        ret = bind(server_socket, (const struct sockaddr *) &name, sizeof(name));

        if (ret == -1) {
                perror("bind");
        }

        /*
            * Prepare for accepting connections. The backlog size is set
            * to 20. So while one request is being processed other requests
            * can be waiting.
            */

	ret = listen(server_socket, 100);
	if (ret == -1) {
		perror("listen");
	}

}

void run_socket() {
	/* This is the main loop for handling connections. */
       	/* Wait for incoming connection. */


       	data_socket = accept(server_socket, NULL, NULL);
       	if (data_socket == -1) {
		perror("accept");
	}


        ret = read(data_socket, buffer, sizeof(buffer));
        if (ret == -1) {
             	perror("read");
        }
       
        std::string message = buffer;
        size_t pos;

        if(strcmp(buffer, "RLVL_CHNG1") == 0) {
                std::cout << " ## Changing runlevel to 1!" << std::endl;
                state::change_state(state::sys_runlevel_1);
        } else if(strcmp(buffer, "RLVL_CHNG2") == 0) {
                std::cout << " ## Changing runlevel to 2!" << std::endl;
                state::change_state(state::sys_runlevel_2);
        } else if(strcmp(buffer, "RLVL_CHNG3") == 0) {
                std::cout << " ## Changing runlevel to 3!" << std::endl;
                state::change_state(state::sys_runlevel_3);
        } else if(strcmp(buffer, "RLVL_CHNG4") == 0) {
                std::cout << " ## Changing runlevel to 4!" << std::endl;
                state::change_state(state::sys_runlevel_4);
        } else if(strcmp(buffer, "RLVL_CHNG5") == 0) {
                std::cout << " ## Changing runlevel to 5!" << std::endl;
                state::change_state(state::sys_runlevel_5);
        } else if(strcmp(buffer, "UNIT_LST") == 0) {
                std::cout << " ## Listing units!" << std::endl;
                if (unit::managed_units.size() >= 1) {
                        strcpy(buffer, unit::managed_units[0].file.c_str());
                        strcat(buffer, "\t");
                        strcat(buffer, std::to_string(unit::managed_units[0].pid).c_str());
                        strcat(buffer, "\n");
                
                        for (int i = 1; i < unit::managed_units.size(); i++) {
                                strcat(buffer, unit::managed_units[i].file.c_str());
                                strcat(buffer, "\t");
                                strcat(buffer, std::to_string(unit::managed_units[i].pid).c_str());
                                strcat(buffer, "\n");
                        }
                }
        } else if ("UNIT_STR") { // unit start
        } else if ("UNIT_STP") { // unit stop
        } else if ("UNIT_RST") { // unit restart (stop and start)
        } else if(strcmp(buffer, "RLVL_LST") == 0) {
                std::cout << " ## Current runlevel: " << state::curr_runlevel << std::endl;
        }

        /* Send buffer. */
        ret = write(data_socket, buffer, sizeof(buffer));
        if (ret == -1) {
                perror("write");
        }

}

void close_socket() {
        close(data_socket);

        /* Close socket. */
        
        close(server_socket);

        /* Unlink the socket. */

	unlink(socket_name);

}

}
