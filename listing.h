#ifndef LISTING_H
#define LISTING_H

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>
#include "mytar.h"

// int v_flag = 0;
// int S_flag = 0;

#define REGULAR_FILE '0'
#define SYM_LINK '2'
#define DIRECTORY '5'



int print_archive(FILE *in_file, int v_flag);
void list_archives(int v_flag);

char file_name[257];

#endif