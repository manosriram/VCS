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
    if (!argv[1]) die("Usage: \n./vcs <options>\n\n<options>: \n-log [Commit logs]\n-view <commit_id> or <tree_id> [View Commit or Tree]\n-commit <commit_message> [Take a snapshot of current repository status]\n-tree <commit_id> [Shows the tree of that commit]\n-file <file_hash> [View contents of file]");
    if (!strcmp(argv[1], "-commit")) {
        char path[526] = ".";
        hash_files_current_dir(path, sizeof path);

        char *tree_hash = write_tree_hash();
        create_commit(tree_hash, argv[2]);
    } else if (!strcmp(argv[1], "-log")) {
        display_all_commits();
    } else if (!strcmp(argv[1], "-tree")) {
        if (!argv[2]) die("Error: No args given.\n");

        get_commit_by_id(argv[2]);
    } else if (!strcmp(argv[1], "-view")) {
        if (!argv[2]) die("Error: No args given.\n");

        view_tree(argv[2]);
    } else if (!strcmp(argv[1], "-file")) {
        char *path = (char *) malloc(sizeof(char) * 264);
        sprintf(path, "./.vcs/objects/%s", argv[2]);
        FILE *file_pt = fopen(path, "r");
        unsigned char *file = read_file(file_pt);

        printf("%s", file);
        fclose(file_pt);
    } else {
        die("Usage: \n./vcs <options>\n\n<options>: \n-log [Commit logs]\n-view <commit_id> or <tree_id> [View Commit or Tree]\n-commit <commit_message> [Take a snapshot of current repository status]\n-tree <commit_id> [Shows the tree of that commit]");
    }
}
