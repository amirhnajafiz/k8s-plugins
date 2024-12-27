#include "../include/handler.h"
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>

// handle function to execute user commands
void handle(int client_socket) {
    char buffer[1024];
    while (true) {
        // read command from socket
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            perror("read");
            break;
        }
        buffer[bytes_read] = '\0';

        // check for exit command
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // create a pipe
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            return;
        }

        // fork a child process
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return;
        }

        if (pid == 0) {
            // child process
            close(pipefd[0]); // close read end of pipe
            dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
            dup2(pipefd[1], STDERR_FILENO); // redirect stderr to pipe
            close(pipefd[1]);

            // execute command
            execl("/bin/sh", "sh", "-c", buffer, (char *)NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        } else {
            // parent process
            close(pipefd[1]); // close write end of pipe

            // read command output from pipe
            ssize_t nbytes;
            while ((nbytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
                buffer[nbytes] = '\0';
                write(client_socket, buffer, nbytes);
            }
            close(pipefd[0]);

            // wait for child process to finish
            waitpid(pid, NULL, 0);
        }
    }
    close(client_socket);
}
