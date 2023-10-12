#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
        char **arguments = malloc(10 * sizeof(char *));
        int arg_count = 0;

        while (token != NULL) {
            arguments[arg_count] = token;
            token = strtok(NULL, " ");
            arg_count++;
        }
        arguments[arg_count] = NULL;

        // Create a child process to execute the command
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (execvp(command, arguments) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }

        free(arguments);
    }

    free(input);
    return 0;
}
