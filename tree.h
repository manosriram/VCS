#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "hash.h"

void write_tree_hash() {
    const char *tree_path = "./.vcs/refs/tree";
    FILE *tree_source, *tree_target;
    tree_source = fopen(tree_path, "r");

    unsigned char *tree_cont = read_file(tree_source);
    uint32_t *hashed = hash_file_buffer(tree_cont);
    char *hashed_tree = (char *) calloc(50, sizeof(char));
    char *tree_hashed = (char *) calloc(264, sizeof(char));
    char hash_a[10], hash_b[10], hash_c[10], hash_d[10], hash_e[10];

    sprintf(hash_a, "%x", hashed[0]);
    sprintf(hash_b, "%x", hashed[1]);
    sprintf(hash_c, "%x", hashed[2]);
    sprintf(hash_d, "%x", hashed[3]);
    sprintf(hash_e, "%x", hashed[4]);
    
    strcat(hashed_tree, hash_a);
    strcat(hashed_tree, hash_b);
    strcat(hashed_tree, hash_c);
    strcat(hashed_tree, hash_d);
    strcat(hashed_tree, hash_e);

    sprintf(tree_hashed, "./.vcs/refs/%s", hashed_tree);
    tree_target = fopen(tree_hashed, "w");

    fprintf(tree_target, "%s", tree_cont);

    fclose(tree_target);
    fclose(tree_source);
    remove(tree_path);
    return;
}

void write_to_tree(const char *hash, const char *path, const char *filename) {
    // path/filename -> location of original file-object
    const char *tree_path = "./.vcs/refs/tree";
    FILE *tree_file = fopen(tree_path, "a");
    
    fprintf(tree_file, "%s %s\n", hash, path);
    fclose(tree_file);
    return;
}
