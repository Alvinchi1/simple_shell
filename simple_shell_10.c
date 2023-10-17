#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

extern char **environ;
int custom_cd(const char *directory)
{
if (directory == NULL)
{
const char *home = getenv("HOME");
if (home == NULL)
{
fprintf(stderr, "cd: $HOME is not set\n");
return -1;
}
directory = home;
}
else if (strcmp(directory, "-") == 0)
{
const char *previous_directory = getenv("OLDPWD");
if (previous_directory == NULL)
{
fprintf(stderr, "cd: OLDPWD not set\n");
return -1;
}
directory = previous_directory;
}
char current_directory[PATH_MAX];
if (getcwd(current_directory, PATH_MAX) == NULL)
{
perror("cd");
return -1;
}
if (chdir(directory) != 0)
{
perror("cd");
return -1;
}
if (setenv("PWD", directory, 1) != 0)
{
perror("cd");
return -1;
}
if (setenv("OLDPWD", current_directory, 1) != 0)
{
perror("cd");
return -1;
}
return 0;
}
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
char *token = strtok(input, " ");
if (token == NULL)
{
continue;
}
char *command = token;
if (strcmp(command, "exit") == 0)
{
free(input);
exit(EXIT_SUCCESS);
}
else if (strcmp(command, "env") == 0)
{
for (char **env = environ; *env != NULL; env++)
{
printf("%s\n", *env);
}
}
else if (strcmp(command, "setenv") == 0)
{
char *var = strtok(NULL, " ");
char *val = strtok(NULL, " ");
if (var != NULL && val != NULL)
{
if (setenv(var, val, 1) != 0)
{
perror("setenv");
}
}
}
else if (strcmp(command, "unsetenv") == 0)
{
char *var = strtok(NULL, " ");
if (var != NULL)
{
if (unsetenv(var) != 0)
{
perror("unsetenv");
}
}
}
else if (strcmp(command, "cd") == 0)
{
char *arg = strtok(NULL, " ");
custom_cd(arg);
}
else
{
pid_t pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}
else if (pid == 0)
{
if (execlp(command, command, NULL) == -1)
{
perror("execlp");
exit(EXIT_FAILURE);
}
}
else
{
int status;
waitpid(pid, &status, 0);
}
}
}
free(input);
return 0;
}
