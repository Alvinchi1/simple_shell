#ifndef SHELL_H
#define SHELL_H

// Function to set environment variables
int custom_setenv(const char *variable, const char *value);

// Function to unset environment variables
int custom_unsetenv(const char *variable);

// Function to execute a single command with variable substitution
int execute_command(const char *command);

#endif
