#include "../include/handler.h"

// handle function to execute user commands
void handle(int client_socket) {
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
        // close the read end of the pipe
        close(pipefd[0]);

        // redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // close the write end of the pipe
        close(pipefd[1]);

        // execute ls command
        execlp("ls", "ls", NULL);

        // if execlp fails, print an error message
        perror("execlp");
        exit(1);
    } else {
        // parent process
        // close the write end of the pipe
        close(pipefd[1]);

        // read the output of ls command from the read end of the pipe
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            // send the output to the client
            if (send(client_socket, buffer, bytes_read, 0) == -1) {
                perror("send");
                break;
            }
        }

        // close the read end of the pipe
        close(pipefd[0]);

        // wait for the child process to exit
        int status;
        waitpid(pid, &status, 0);
    }
}
