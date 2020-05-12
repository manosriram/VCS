#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}
void listdir(char *path, size_t size) {
    struct dirent *de;
    FILE *file;
    FILE *new_file_creator;
    char hashed_file[255];
    DIR *dir;
    struct dirent *entry;
    size_t len = strlen(path);

    if (!(dir = opendir(path))) {
        fprintf(stderr, "path not found: %s: %s\n",
                path, strerror(errno));
        return;
    }

    puts(path);
    while ((entry = readdir(dir)) != NULL) {
            printf("123  ");
        char *name = entry->d_name;
        if (entry->d_type == DT_DIR) {
            if (file_ignored(name) && entry->d_name[0] == '.')
                continue;
            if (len + strlen(name) + 2 > size) {
                fprintf(stderr, "path too long: %s/%s\n", path, name);
            } else {
                path[len] = '/';
                strcpy(path + len + 1, name);
                listdir(path, size);
                path[len] = '\0';
            }
        } else {
            char combined[100];
            sprintf(combined, "%s/%s", path, name);

            file = fopen(combined, "r");

            unsigned char *file_buffer = read_file(file);
            uint32_t *hashed = hash_file_buffer(file_buffer);

            char hash_a[10], hash_b[10], hash_c[10], hash_d[10], hash_e[10];

            sprintf(hash_a, "%x", hashed[0]);
            sprintf(hash_b, "%x", hashed[1]);
            sprintf(hash_c, "%x", hashed[2]);
            sprintf(hash_d, "%x", hashed[3]);
            sprintf(hash_e, "%x", hashed[4]);

            char *result = calloc(40, sizeof(char));
            char *file_name = calloc(38, sizeof(char));

            strcat(result, hash_a);
            strcat(result, hash_b);
            strcat(result, hash_c);
            strcat(result, hash_d);
            strcat(result, hash_e);

            snprintf(hashed_file, sizeof(hashed_file), "./.vcs/objects/%s", result);
            new_file_creator = fopen(hashed_file, "w");
            fprintf(new_file_creator, "%s", file_buffer);

            free(result);
            free(file_name);
        }
    }
    closedir(dir);
    fclose(file);
    fclose(new_file_creator);
}


int main(void) {
    char path[1024] = ".";
    listdir(path, sizeof path);
}
