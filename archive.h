#ifndef MYTAR_H
#define MYTAR_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

char mem_block[512];

typedef struct header header;
typedef header* header_ptr;
typedef struct stat* stat_ptr;

struct __attribute__ ((__packed__)) header{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];	
};

// struct dirent *direnty;
// struct passwd *password;
// struct group *group;
// //struct header *headerer;

// typedef struct header header;
// typedef header* headerer_ptr;
// typedef struct stat stat;

// typedef stat *stat_ptr;

// typedef struct header {
//     char name[100];
//     char mode[8];
//     char uid[8];
//     char gid[8];
//     char size[12];
//     char mtime[12];
//     char chksum[8];
//     char typeflag;
//     char linkname[100];
//     char magic[6];
//     char version[2];
//     char uname[32];
//     char gname[32];
//     char devmajor[8];
//     char devminor[8];
//     char prefix[155];
//     char padding[12];	
// } header;

void populate_header(stat_ptr sp);
void populate_header_buffer(headerer_ptr hp);
void manage_file(FILE file);
void archive_file(FILE file);
void expand_directory(FILE file);
void array_to_buffer(char* arr);
void write_to_buffer(int ch);
void decToOctal(int n);

#endif
