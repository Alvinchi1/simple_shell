#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
char *input = NULL;
size_t len = 0;
ssize_t nread;
while (1)
{
printf("simple_shell$ ");
nread = getline(&input, &len, stdin);
if (nread == -1)
{
if (feof(stdin))
{
printf("\n");
break;
}
else
{
perror("getline");
exit(EXIT_FAILURE);
}
}
input[strcspn(input, "\n")] = '\0';
pid_t pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}
else if (pid == 0)
{
if (execve(input, NULL, NULL) == -1)
{
perror("execve");
exit(EXIT_FAILURE);
}
}
else
{
int status;
waitpid(pid, &status, 0);
}
}
free(input);
return 0;
}
