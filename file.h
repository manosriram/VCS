#include <sys/stat.h>
#include <stdbool.h>

/*
 * To be called like this: file_exists(filename); (filename is of (char *) type)
*/
unsigned int file_exists(char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

unsigned int file_ignored(char *filename) {
    FILE* ignored;
    ignored = fopen(".vcs", "r");
    const size_t line_size = 256;

    char* line = (char *)malloc(line_size);
    while (fgets(line, line_size, ignored) != NULL)  {
        if (strcmp(line, filename) == 10) return 1;
    }
    return 0;
}
