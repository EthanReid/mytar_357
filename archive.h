#ifndef MYTAR_H
#define MYTAR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
void populate_header(char *name, stat_ptr sp);
void manage_file(char *name);
void archive_file(char *name, stat_ptr sp);
void expand_directory(char *name);
void array_to_buffer(char* arr);
void write_to_buffer(int ch);
void decToOctal(int n);
char *concat_str(char *str1, char *str2);

#endif
