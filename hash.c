#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "hash.h"
#include "./file.h"

uint32_t *read_file_contents_and_hash(FILE *fp) {
    char c;
    unsigned char *buf = (unsigned char *) malloc(sizeof(unsigned char) * 100000);
    int in = -1;
    for (c = getc(fp); c != EOF; c = getc(fp)) {
        buf[++in] = c;
    }

    uint32_t *hg = SHA1(buf);
    for (int t=0;t<5;++t) printf("%x", hg[t]);

    printf("\n");
    return hg;
}

void hash_files_current_dir() {
   struct dirent *de;
   DIR *dr = opendir(".");
   FILE *file;
  while ((de = readdir(dr)) != NULL) {
    char *filename = de->d_name;
          // de->d_name: dir_name
    if (!file_ignored(filename) && de->d_name[0] != '.') {
          printf("%s\n", de->d_name);
          file = fopen(de->d_name, "r");
          read_file_contents_and_hash(file);
    }
  }

  fclose(file);
  closedir(dr); 
}

int main() {
    hash_files_current_dir();
}
