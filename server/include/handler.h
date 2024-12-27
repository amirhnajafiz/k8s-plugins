#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// handle function to execute user commands
void handle(int client_socket);
