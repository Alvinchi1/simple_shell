#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 256

int main() {
char command[MAX_COMMAND_LENGTH];
while (1) {
printf("simple_shell> ");
if (fgets(command, sizeof(command), stdin) == NULL) {
printf("\nExiting shell\n");
break;
}
command[strcspn(command, "\n")] = '\0';
pid_t pid = fork();
if (pid < 0) {
perror("Fork failed");
exit(EXIT_FAILURE);
}
if (pid == 0)
{
if (execlp(command, command, NULL) == -1)
perror("Command execution failed");
exit(EXIT_FAILURE);
}
}
else
{
int status;
waitpid(pid, &status, 0);
if (WIFEXITED(status))
{
int exit_status = WEXITSTATUS(status);
if (exit_status != 0)
{
printf("Command failed with exit status %d\n", exit_status);
}
}
}
}
return 0;
}
