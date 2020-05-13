#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "tree.h"
#include "hash.h"
#include "commit.h"
void die(char* message){
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (!argv[1]) die("No args provided.\n");
    if (!strcmp(argv[1], "commit")) {
        char path[526] = ".";
        hash_files_current_dir(path, sizeof path);

        char *tree_hash = write_tree_hash();
        create_commit(argv[2], tree_hash, argv[3]);
    } else if (!strcmp(argv[1], "log")) {
        display_all_commits();
    } else if (!strcmp(argv[1], "get")) {
        if (!argv[2]) die("Error: No ID given.\n");

        get_commit_by_id(argv[2]);
    }
}
