#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include "tree.h"
#include "file.h"
#define rotateleft(x,n) ((x<<n) | (x>>(32-n)))

typedef struct Hash {
    uint32_t *hashed_buffer;
    char hashed_file_path[255], combined[64];
    char *hash_result;
} Hash;

void write_to_tree(Hash *_hash, const char *filename);

uint32_t *SHA1(const unsigned char * str1) {
    static uint32_t h[5];
    uint32_t a,b,c,d,e,f,k,temp;

    h[0] = 0x67452301;
    h[1] = 0xEFCDAB89;
    h[2] = 0x98BADCFE;
    h[3] = 0x10325476;
    h[4] = 0xC3D2E1F0;

    unsigned char * str;
    str = (unsigned char *)malloc(strlen((const char *)str1)+100);
    strcpy((char *)str,(const char *)str1);

    int current_length = strlen((const char *)str);
    int original_length = current_length;

    str[current_length] = 0x80;
    str[current_length + 1] = '\0';

    char ic = str[current_length];

    current_length++;
    int ib = current_length % 64;

    int i, j;

    if(ib<56)
        ib = 56-ib;
    else
        ib = 120 - ib;

    for(i=0;i < ib;i++) {
        str[current_length]=0x00;
        current_length++;
    }

    str[current_length + 1]='\0';

    for(i=0;i<6;i++) {
        str[current_length]=0x0;
        current_length++;
    }

    str[current_length] = (original_length * 8) / 0x100 ;
    current_length++;
    str[current_length] = (original_length * 8) % 0x100;
    current_length++;
    str[current_length+i]='\0';

    int number_of_chunks = current_length/64;
    uint32_t word[80];

    for(i=0;i<number_of_chunks;i++) {
        for(j=0;j<16;j++) {
            word[j] =
                str[i*64 + j*4 + 0] * 0x1000000 + str[i*64 + j*4 + 1] * 0x10000 +
                str[i*64 + j*4 + 2] * 0x100 + str[i*64 + j*4 + 3];
        }
        for(j=16;j<80;j++) {
            word[j] = rotateleft((word[j-3] ^ word[j-8] ^ word[j-14] ^ word[j-16]),1);
        }
        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        e = h[4];
        for(int m=0;m<80;m++) {
            if(m<=19) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if(m<=39) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if(m<=59) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            temp = (rotateleft(a,5) + f + e + k + word[m]) & 0xFFFFFFFF;
            e = d;
            d = c;
            c = rotateleft(b,30);
            b = a;
            a = temp;

        }

        h[0] = h[0] + a;
        h[1] = h[1] + b;
        h[2] = h[2] + c;
        h[3] = h[3] + d;
        h[4] = h[4] + e;

    }
    return h;
}

/*
 * Hash files by it's content and returns hashed content in 5 parts.
 * Return an uint32_t type.
 */
uint32_t *hash_file_buffer(const unsigned char *file_buffer) {
    uint32_t *hashed_buffer = SHA1(file_buffer);
    return hashed_buffer;
}

char *combine_hash(uint32_t *hashed) {
    char hash_a[10], hash_b[10], hash_c[10], hash_d[10], hash_e[10];

    sprintf(hash_a, "%x", hashed[0]);
    sprintf(hash_b, "%x", hashed[1]);
    sprintf(hash_c, "%x", hashed[2]);
    sprintf(hash_d, "%x", hashed[3]);
    sprintf(hash_e, "%x", hashed[4]);

    char *combined_hash;
    combined_hash = (char *) malloc(sizeof(char) * 64);
    strcat(combined_hash, hash_a);
    strcat(combined_hash, hash_b);
    strcat(combined_hash, hash_c);
    strcat(combined_hash, hash_d);
    strcat(combined_hash, hash_e);

    return combined_hash;
}

void hash_files_current_dir(char *path, size_t size) {
    Hash *_hash = (Hash *) malloc(sizeof(Hash));
    File *_file = (File *) malloc(sizeof(File));

    struct dirent *de;
    FILE *file;
    DIR *dir;
    struct dirent *entry;
    size_t len = strlen(path);

    if (!(dir = opendir(path)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;
        if (file_ignored(name)) continue;
        if (entry->d_type == DT_DIR) {
            if (len + strlen(entry->d_name) + 2 > size) {
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
            // printf("%s\n", _hash->combined);

            _file->file_buffer = read_file(file);
            uint32_t *hashed = hash_file_buffer(_file->file_buffer);

            _hash->hash_result = combine_hash(hashed);
            snprintf(_hash->hashed_file_path, sizeof(_hash->hashed_file_path), "./.vcs/objects/%s", _hash->hash_result);

            printf("%s\n", _hash->combined);
            remove(_hash->hashed_file_path);
            _file->new_file_creator = fopen(_hash->hashed_file_path, "w");
            fprintf(_file->new_file_creator, "%s", _file->file_buffer);
            chmod(_hash->hashed_file_path, S_IRUSR | S_IRGRP | S_IROTH);

            write_to_tree(_hash, name);
        }
    }
    closedir(dir);
    fclose(file);
    fclose(_file->new_file_creator);
}
