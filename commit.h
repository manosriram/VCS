#pragma once
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>

typedef struct Commit {
    char *commit_id;
    char *message, *tree;
    long long int created;
    FILE *commit_file;
    char *buffer;
} Commit;

long long int current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long int milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}

void create_commit(char *tree_hash, char *message) {

    // Fill Commit Structure.
    Commit *commit = (Commit *) malloc(sizeof(Commit));
    commit->buffer = (char *) malloc(sizeof(char) * 16384);
    commit->created = current_timestamp();
    commit->message = message;
    commit->tree = tree_hash;

    // Write Commit file path.
    char *commit_path = (char *)malloc(sizeof(char) * 64);

    /* Writes Content of Commit. Format is as follows:
     *
     *<tree_hash> <name_id>
     *
     *Commit <message>
     *
     */

    time_t a = commit->created/1000;
    sprintf(commit->buffer, "%s %s\n%s",  tree_hash, ctime(&a), commit->message);

    uint32_t *hashed_content = SHA1((const unsigned char *) commit->buffer);
    commit->commit_id = combine_hash(hashed_content);
    sprintf(commit_path, "./.vcs/refs/%s", commit->commit_id);
    commit->commit_file = fopen(commit_path, "w");
    fprintf(commit->commit_file, "%s\nCommit %s\n", commit->buffer, commit->commit_id);
    FILE *hash_file = fopen("./.vcs/refs/commit_hist", "a");
    fprintf(hash_file, "%s\nCommit %s\n\n", commit->buffer, commit->commit_id);


    fclose(commit->commit_file);
    free(commit->buffer);
    free(commit);
    return;
}

void display_all_commits() {
    FILE *file = fopen("./.vcs/refs/commit_hist", "r");

    char line[3424];
    while (fgets(line, sizeof(line), file)) printf("%s", line);
    return;
}

void get_commit_by_id(char *commit_id) {
    char path[264];
    sprintf(path, "./.vcs/refs/%s", commit_id);
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Unknown commit_id\n");
        return;
    }

    char line[3424];
    char buff[41];
    while (fgets(line, sizeof(line), file)) {
        int i = -1;
        while (line[++i] != ' ') {
            buff[i] = line[i];
        }
        buff[40] = '\0';
        break;
    }
    view_tree(buff);
    fclose(file);
    return;
}

