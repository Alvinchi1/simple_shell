#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// Custom getline function
ssize_t custom_getline(char **lineptr, size_t *n, int fd) {
    static char buffer[BUFFER_SIZE];
    static size_t buf_pos = 0;
    static ssize_t bytes_read = 0;

    if (fd < 0 || !lineptr || !n) {
        return -1;
    }

    if (*lineptr == NULL || *n == 0) {
        *lineptr = (char *)malloc(BUFFER_SIZE);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = BUFFER_SIZE;
    }

    ssize_t total_bytes_read = 0;
    int eof_flag = 0;

    while (1) {
        if (buf_pos >= bytes_read) {
            bytes_read = read(fd, buffer, BUFFER_SIZE);
            buf_pos = 0;

            if (bytes_read <= 0) {
                if (total_bytes_read == 0) {
                    free(*lineptr);
                    *lineptr = NULL;
                }
                return total_bytes_read > 0 ? total_bytes_read : bytes_read;
            }
        }

        char current_char = buffer[buf_pos++];
        (*lineptr)[total_bytes_read++] = current_char;

        if (total_bytes_read >= *n - 1) {
            // Reallocate more memory if needed
            *n *= 2;
            char *new_lineptr = (char *)realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                free(*lineptr);
                *lineptr = NULL;
                return -1;
            }
            *lineptr = new_lineptr;
        }

        if (current_char == '\n') {
            eof_flag = 1;
            break;
        }
    }

    (*lineptr)[total_bytes_read] = '\0';

    return total_bytes_read - eof_flag;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t n;

    while ((n = custom_getline(&line, &len, STDIN_FILENO)) > 0) {
        printf("Read %zd bytes: %s", n, line);
    }

    free(line);
    return 0;
}
