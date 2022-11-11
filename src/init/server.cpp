#include "server.h"

namespace server {

char* socket_name = "/var/run/init.socket\0";
unsigned short socket_buffer_size = 1024;
struct sockaddr_un name;
int server_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
int data_socket;
char* buffer;
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
        buffer = malloc(socket_buffer_size);

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

	ret = listen(server_socket, 20);
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
        /* Send buffer. */

        std::cout << buffer;
        ret = write(data_socket, buffer, sizeof(buffer));
        if (ret == -1) {
                perror("write");
        }
}

void close_socket() {

        free(buffer);

        close(data_socket);

        /* Close socket. */
        
        close(server_socket);

        /* Unlink the socket. */

	unlink(socket_name);

}

}
