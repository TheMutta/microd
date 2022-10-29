#include "postinit.h"

inline void launch_session_manager() {
	printf("Launching session manager...");
	
	pid_t pid;
	pid = fork();
	if (pid == 0) {
		execlp("busybox", "setsid", "cttyhack", "sh");
		perror("execlp");
		panic("Failed to start session manager!");
	} else {
		ok("Session manager launched. Ceeding control.");
	}
}
