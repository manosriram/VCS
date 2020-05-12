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
uint32_t *hash_file_buffer(const unsigned char *file_buffer) {
    uint32_t *hashed_buffer = SHA1(file_buffer);
    return hashed_buffer;
}


void hash_files_current_dir(char *path, size_t size) {
    Hash *_hash;
    File *_file;
    _hash = (Hash *) malloc(sizeof(Hash));
    _file = (File *) malloc(sizeof(File));

    struct dirent *de;
    FILE *file;
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
            sprintf(_hash->combined, "%s/%s", path, name);

            file = fopen(_hash->combined, "r");
            // printf("%s\n", combined);

            _file->file_buffer = read_file(file);
            uint32_t *hashed = hash_file_buffer(_file->file_buffer);

            char hash_a[10], hash_b[10], hash_c[10], hash_d[10], hash_e[10];

            sprintf(hash_a, "%x", hashed[0]);
            sprintf(hash_b, "%x", hashed[1]);
            sprintf(hash_c, "%x", hashed[2]);
            sprintf(hash_d, "%x", hashed[3]);
            sprintf(hash_e, "%x", hashed[4]);

            _hash->hash_result = calloc(40, sizeof(char));

            strcat(_hash->hash_result, hash_a);
            strcat(_hash->hash_result, hash_b);
            strcat(_hash->hash_result, hash_c);
            strcat(_hash->hash_result, hash_d);
            strcat(_hash->hash_result, hash_e);

            snprintf(_hash->hashed_file_path, sizeof(_hash->hashed_file_path), "./.vcs/objects/%s", _hash->hash_result);

            remove(_hash->hashed_file_path);
            _file->new_file_creator = fopen(_hash->hashed_file_path, "w");
            fprintf(_file->new_file_creator, "%s", _file->file_buffer);
            chmod(_hash->hashed_file_path, S_IRUSR | S_IRGRP | S_IROTH);

            if (!is_directory("./.vcs/refs"))
                mkdir("./.vcs/refs", 0777);

            write_to_tree(_hash, name);

            free(_hash->hash_result);
        }
    }
    closedir(dir);
    fclose(file);
    fclose(_file->new_file_creator);
}

int main() {
    char path[1024] = ".";
    hash_files_current_dir(path, sizeof path);

    write_tree_hash();
}
