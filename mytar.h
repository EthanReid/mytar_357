#ifndef MYTAR_H
#define MYTAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "archive.h"
#include "blockBuffer.h"

// struct header head;

extern FILE *out_file;
FILE *in_file;
int argc_val;
//extern char *argv_copy;


// typedef struct Header {
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
// } Header;



#endif
