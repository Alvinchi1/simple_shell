#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;  // Declare the external environment variable

int main(void)
{
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
            // Check for an argument (status code)
            token = strtok(NULL, " ");
            if (token != NULL) {
                int status = atoi(token);
                free(input);
                exit(status);
            } else {
                free(input);
                exit(EXIT_SUCCESS);  // Exit the shell with success code
            }
        } else if (strcmp(command, "env") == 0) {
            // Print the environment variables
            for (char **env = environ; *env != NULL; env++) {
                printf("%s\n", *env);
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

