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



#define REGULAR_FILE '0'
#define SYM_LINK '2'
#define DIRECTORY '5'




void list_archives(char *file_name, int v_flag, header *head);
int print_archive(FILE *in_file, int v_flag, char **argv);

char file_name[257];

#endif