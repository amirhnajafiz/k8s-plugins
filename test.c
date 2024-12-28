#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main() {
    FILE *shell;
    char command[BUFFER_SIZE];
    char output[BUFFER_SIZE];

    // Open a pipe to /bin/sh in write mode
    shell = popen("/bin/sh", "w");
    if (shell == NULL) {
        fprintf(stderr, "popen() failed: %s\n", strerror(errno));
        exit(1);
    }

    // Open a pipe to /bin/sh in read mode
    FILE *shell_read = popen("/bin/sh", "r");
    if (shell_read == NULL) {
        fprintf(stderr, "popen() failed: %s\n", strerror(errno));
        pclose(shell);
        exit(1);
    }

    while (1) {
        printf("Enter a command (or 'exit' to quit): ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            break;
        }

        // Write the command to the shell
        fprintf(shell, "%s\n", command);
        fflush(shell);

        // Read and print the output
        while (fgets(output, sizeof(output), shell_read) != NULL) {
            printf("%s", output);
        }
    }

    // Close the pipes
    pclose(shell);
    pclose(shell_read);
    return 0;
}
