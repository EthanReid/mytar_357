#ifndef LISTING_TWO_H
#define LISTING_TWO_H


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


#define STRUCT_CHKSUM 8
#define STRUCT_MAGIC 6
#define STRUCT_VERSION 2
#define CHKSUM_THRESHOLD 148

#define FLAG_T 't'
#define FLAG_V 'v'
#define FLAG_F 'f'

struct header {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char chksum[8];
	char typeflag[1];
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

void print_usage_message(char *program_name);
void handle_invalid_flag_selection(char *program_name, char invalid_flag);
void print_archive_v_flag(struct header *head, char permissions_string[10]);
void print_archive(int fd, char *argv[], int argc, char curr_flag);
int t_arg(char *file, char type, char *argv[], int argc);
int is_header_valid(struct header *head);
char *path_concatenate(char *prefix, char *name);



#endif
