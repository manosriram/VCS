#pragma once
#include <stdio.h>
#include <sys/time.h>
#include "hash.h"

typedef struct Commit {
    char *commit_id;
    char *message, *tree;
    long long int created;
} Commit;

long long int current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long int milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}

void create_commit(char *tree_hash, char *message) {
    FILE *temp = fopen("./.vcs/refs/commit", "w");

    unsigned char *cont;
    long long int now = current_timestamp();
    sprintf((char *)cont, "%s %lld\n\n%s",  tree_hash, now, message);

    uint32_t *hashed_content = SHA1(cont);

    return;
}
