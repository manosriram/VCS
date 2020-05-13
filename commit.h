#pragma once
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

typedef struct Commit {
    char *commit_id;
    char *message, *tree;
    long long int created;
    FILE *commit_file;
    char *buffer;
    char *name_id;
} Commit;

long long int current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long int milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}

void create_commit(char *name, char *tree_hash, char *message) {

    // Fill Commit Structure.
    Commit *commit = (Commit *) malloc(sizeof(Commit));
    commit->buffer = (char *) malloc(sizeof(char) * 16384);
    commit->created = current_timestamp();
    commit->message = message;
    commit->tree = tree_hash;
    commit->name_id = name;

    // Write Commit file path.
    char *commit_path = (char *)malloc(sizeof(char) * 64);

    /* Writes Content of Commit. Format is as follows:
     *
     *<tree_hash> <created> <name_id>
     *
     *<message>
     *
     */

    sprintf(commit->buffer, "%s %lld %s\n\n%s",  tree_hash, commit->created, commit->name_id, commit->message);

    uint32_t *hashed_content = SHA1((const unsigned char *) commit->buffer);
    commit->commit_id = combine_hash(hashed_content);
    sprintf(commit_path, "./.vcs/refs/%s", commit->commit_id);
    commit->commit_file = fopen(commit_path, "w");
    fprintf(commit->commit_file, "%s", commit->buffer);

    fclose(commit->commit_file);
    free(commit->buffer);
    free(commit);
    return;
}
