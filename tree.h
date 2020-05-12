#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "hash.h"

 typedef struct Tree {
    const char *path;
    const unsigned char *buffer;
    char *hash, *hashed_path;

    FILE *tree_source, *tree_target;
} Tree;

void write_tree_hash(void) {
    Tree *tree = (Tree *) malloc(sizeof(Tree));

    tree->hash = (char *) calloc(100, sizeof(char));
    tree->hashed_path = (char *) calloc(264, sizeof(char));
    tree->path = "./.vcs/refs/tree";
    tree->tree_source = fopen(tree->path, "r");
    tree->buffer = read_file(tree->tree_source);

    uint32_t *hashed = hash_file_buffer(tree->buffer);
    char hash_a[10], hash_b[10], hash_c[10], hash_d[10], hash_e[10];

    sprintf(hash_a, "%x", hashed[0]);
    sprintf(hash_b, "%x", hashed[1]);
    sprintf(hash_c, "%x", hashed[2]);
    sprintf(hash_d, "%x", hashed[3]);
    sprintf(hash_e, "%x", hashed[4]);
    
    strcat(tree->hash, hash_a);
    strcat(tree->hash, hash_b);
    strcat(tree->hash, hash_c);
    strcat(tree->hash, hash_d);
    strcat(tree->hash, hash_e);

    sprintf(tree->hashed_path, "./.vcs/refs/%s", tree->hash);
    tree->tree_target = fopen(tree->hashed_path, "w");

    fprintf(tree->tree_target, "%s", tree->buffer);

    fclose(tree->tree_target);
    fclose(tree->tree_source);
    free(tree);
    remove(tree->path);
    return;
}

void write_to_tree(Hash *_hash, const char *filename) {
    // path/filename -> location of original file-object
   
    const char *tree_path = "./.vcs/refs/tree";
    FILE *tree_file = fopen(tree_path, "a");
    
    fprintf(tree_file, "%s %s\n", _hash->hash_result, _hash->combined);
    fclose(tree_file);
    return;
}
