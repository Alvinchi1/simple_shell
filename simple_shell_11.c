#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

extern char **environ;  // Declare the external environment variable

// Function to set environment variables
int custom_setenv(const char *variable, const char *value) {
    if (variable == NULL || value == NULL) {
        fprintf(stderr, "setenv: Missing arguments\n");
        return -1;
    }

    if (setenv(variable, value, 1) != 0) {
        fprintf(stderr, "setenv: Failed to set environment variable\n");
        return -1;
    }

    return 0;
}

// Function to unset environment variables
int custom_unsetenv(const char *variable) {
    if (variable == NULL) {
        fprintf(stderr, "unsetenv: Missing argument\n");
        return -1;
    }

    if (unsetenv(variable) != 0) {
        fprintf(stderr, "unsetenv: Failed to unset environment variable\n");
        return -1;
    }

    return 0;
}

// Function to execute a single command
int execute_command(const char *command) {
    if (command == NULL) {
        return 0;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (execlp(command, command, NULL) == -1) {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}

int main(void) {
    char *input = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1) {
        printf("simple_shell$ ");  // Display the prompt
        nread = getline(&input, &len, stdin);

        if (nread == -1) {
            if (feof(stdin)) {
                printf("\n");  // Handle "Ctrl+D" - end of file condition
                break;
            } else {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        // Remove the newline character
        input[strcspn(input, "\n")] = '\0';

        // Tokenize the input into individual commands based on ';'
        char *token = strtok(input, ";");
        while (token != NULL) {
            // Trim leading and trailing whitespaces from the command
            char *command = token;
            while (*command == ' ' || *command == '\t') {
                command++;
            }
            char *end = command + strlen(command) - 1;
            while (end > command && (*end == ' ' || *end == '\t')) {
                *end-- = '\0';
            }

            if (strlen(command) > 0) {
                // Execute the individual command
                execute_command(command);
            }

            token = strtok(NULL, ";");
        }
    }

    free(input);
    return 0;
}
