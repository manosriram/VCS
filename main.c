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

int main() {
    char path[526] = ".";
    hash_files_current_dir(path, sizeof path);

    char *tree_hash = write_tree_hash();
    create_commit("unique name", tree_hash, "Initial Commit");
}

