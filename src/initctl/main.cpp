#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#include "../config.h"

char* socket_name = "/var/run/init.socket";
unsigned short socket_buffer_size = 1024;

int main(int argc, char** argv) {
        std::cout << "Hello world from initctl " << version << std::endl;
        struct sockaddr_un addr;
        int ret;
        int data_socket;
        char buffer[socket_buffer_size];

        /* Create local socket. */

        data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
        if (data_socket == -1) {
                perror("socket");
                exit(EXIT_FAILURE);
        }

        /*
            * For portability clear the whole structure, since some
            * implementations have additional (nonstandard) fields in
            * the structure.
            */

        memset(&addr, 0, sizeof(addr));

        /* Connect socket to socket address. */

        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, socket_name, sizeof(addr.sun_path) - 1);

        ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(addr));
        if (ret == -1) {
                fprintf(stderr, "The server is down.\n");
                exit(EXIT_FAILURE);
        }

        /* Send arguments. */
        
        if (argc > 1) {
                strcpy(buffer, argv[1]);
                buffer[sizeof(buffer) - 1] = 0;
                ret = write(data_socket, buffer, strlen(buffer) + 1);
                if (ret == -1) {
                        perror("write");
                        exit(EXIT_FAILURE);
                }
        }

        kill(1, SIGCONT);

        /* Receive result. */

        ret = read(data_socket, buffer, sizeof(buffer));
        if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
        }

        /* Ensure buffer is 0-terminated. */

        buffer[sizeof(buffer) - 1] = 0;

        std::cout << buffer << std::endl;

        /* Close socket. */

        close(data_socket);

        exit(EXIT_SUCCESS);
        return 0;
}
