#include <stdio.h>
#include <stdlib.h>

// Custom line-reading function
ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream) {
    if (!lineptr || !n || !stream) {
        return -1;
    }

    ssize_t total_bytes_read = 0;
    int current_char;
    int eof_flag = 0;

    if (*lineptr == NULL || *n == 0) {
        *lineptr = (char *)malloc(128); // Start with an initial buffer size
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    while ((current_char = fgetc(stream)) != EOF) {
        (*lineptr)[total_bytes_read++] = (char)current_char;

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

    if (total_bytes_read == 0 && eof_flag == 0) {
        return -1; // Nothing was read (e.g., reached end of file)
    }

    return total_bytes_read;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t n;

    while ((n = custom_getline(&line, &len, stdin)) > 0) {
        printf("Read %zd bytes: %s", n, line);
    }

    free(line);
    return 0;
}
