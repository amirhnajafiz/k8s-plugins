#include "../include/handler.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <sys/select.h>

using namespace std;

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

        cout << "Command executed" << endl;

        // Set up the file descriptor set
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(fileno(shell_read), &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 10; // Increase timeout to 10 seconds
        timeout.tv_usec = 0;

        cout << "Waiting for command output..." << endl;

        int retval = select(fileno(shell_read) + 1, &read_fds, NULL, NULL, &timeout);

        if (retval == -1) {
            std::cerr << "Error in select: " << strerror(errno) << std::endl;
        } else if (retval == 0) {
            std::cerr << "Timeout occurred! No data available." << std::endl;
        } else {
            cout << "Data available, reading from pipe..." << endl;
            // Read the command output from the pipe
            while ((nbytes = read(fileno(shell_read), buffer, sizeof(buffer) - 1)) > 0) {
                buffer[nbytes] = '\0'; // Null-terminate the buffer
                ssize_t obytes = send(client_socket, buffer, nbytes, 0); // Send the output back to the client
                if (obytes < 0) {
                    std::cerr << "Error sending command output: " << strerror(errno) << std::endl;
                    break;
                } else if (obytes == 0) {
                    std::cerr << "Connection closed by client" << std::endl;
                    break;
                }
                cout << "Command output: " << buffer << endl;
            }

            if (nbytes < 0) {
                std::cerr << "Error reading from pipe: " << strerror(errno) << std::endl;
            }
        }

        cout << "Command output sent" << endl;
    }

    // Close the pipes
    pclose(shell);
    pclose(shell_read);
    close(client_socket);
}
