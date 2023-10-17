#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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

        // Tokenize the input into command and arguments
        char *token = strtok(input, " ");
        if (token == NULL) {
            continue;  // Empty line, prompt again
        }

        char *command = token;

        if (strcmp(command, "exit") == 0) {
            free(input);
            exit(EXIT_SUCCESS);  // Exit the shell with success code
        } else if (strcmp(command, "env") == 0) {
            // Print the environment variables
            for (char **env = environ; *env != NULL; env++) {
                printf("%s\n", *env);
            }
        } else if (strcmp(command, "setenv") == 0) {
            // Handle the setenv command
            char *var = strtok(NULL, " ");
            char *val = strtok(NULL, " ");
            if (var != NULL && val != NULL) {
                custom_setenv(var, val);
            }
        } else if (strcmp(command, "unsetenv") == 0) {
            // Handle the unsetenv command
            char *var = strtok(NULL, " ");
            if (var != NULL) {
                custom_unsetenv(var);
            }
        } else {
            // Create a child process to execute the command
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
        }
    }

    free(input);
    return 0;
}
