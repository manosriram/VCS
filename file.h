#pragma once

#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>

/*
 * To be called like this: file_exists(filename); (filename is of (char *) type)
 */
unsigned int file_exists(char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

/*
 * Checks if the file is ignored or not.
 * Return an unsigned int, 1 if ignored, 0 if not.
 */
unsigned int file_ignored(char *filename) {
    FILE* ignored;
    ignored = fopen(".vcsignore", "r");
    const size_t line_size = 256;

    char* line = (char *)malloc(line_size);
    while (fgets(line, line_size, ignored) != NULL)  {
        if (strcmp(line, filename) == 10) return 1;
    }
    return 0;
}

/*
 * Checks If a given path is a directory or not.
 */
int is_directory(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

/*
 * Reads a file and returns it's content as `unsigned char *`.
 * Must be called utmost once for one file-pointer.
 */
unsigned char *read_file(FILE *file_pointer) {
    char c;
    unsigned char *file_buffer = (unsigned char *) malloc(sizeof(unsigned char) * 100000);
    int in = -1;
    for (c = getc(file_pointer); c != EOF; c = getc(file_pointer))
        file_buffer[++in] = c;
    return file_buffer;
}

