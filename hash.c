#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "hash.h"
#include "file.h"
#include "tree.h"

/*
 * Hash files by it's content and returns hashed content in 5 parts.
 * Return an uint32_t type.
 */
uint32_t *hash_file_buffer(unsigned char *file_buffer) {
    uint32_t *hashed_buffer = SHA1(file_buffer);
    return hashed_buffer;
}


void hash_files_current_dir(char *path, size_t size) {
    struct dirent *de;
    FILE *file;
    FILE *new_file_creator;
    DIR *dir;
    struct dirent *entry;
    size_t len = strlen(path);

    if (!(dir = opendir(path)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        char *name = entry->d_name;
        if (file_ignored(name)) continue;
        if (entry->d_type == DT_DIR) {
            if (len + strlen(name) + 2 > size) {
                fprintf(stderr, "path too long: %s/%s\n", path, name);
            } else {
                path[len] = '/';
                strcpy(path + len + 1, name);
                hash_files_current_dir(path, size);
                path[len] = '\0';
            }
        } else {
            char hashed_file_path[255];
            char combined[100];
            sprintf(combined, "%s/%s", path, name);

            file = fopen(combined, "r");
            // printf("%s\n", combined);

            unsigned char *file_buffer = read_file(file);
            uint32_t *hashed = hash_file_buffer(file_buffer);

            char hash_a[10], hash_b[10], hash_c[10], hash_d[10], hash_e[10];

            sprintf(hash_a, "%x", hashed[0]);
            sprintf(hash_b, "%x", hashed[1]);
            sprintf(hash_c, "%x", hashed[2]);
            sprintf(hash_d, "%x", hashed[3]);
            sprintf(hash_e, "%x", hashed[4]);

            char *result = calloc(40, sizeof(char));

            strcat(result, hash_a);
            strcat(result, hash_b);
            strcat(result, hash_c);
            strcat(result, hash_d);
            strcat(result, hash_e);

            snprintf(hashed_file_path, sizeof(hashed_file_path), "./.vcs/objects/%s", result);

            remove(hashed_file_path);
            new_file_creator = fopen(hashed_file_path, "w");
            fprintf(new_file_creator, "%s", file_buffer);
            chmod(hashed_file_path, S_IRUSR | S_IRGRP | S_IROTH);

            if (!is_directory("./.vcs/refs"))
                mkdir("./.vcs/refs", 0777);

            write_to_tree(result, combined, name);

            free(result);
        }
    }
    closedir(dir);
    fclose(file);
    fclose(new_file_creator);
}

int main() {
    char path[1024] = ".";
    hash_files_current_dir(path, sizeof path);

    write_tree_hash();
}
