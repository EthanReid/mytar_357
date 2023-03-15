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


#define CHKSUM 8
#define CHKSUM_OFF 148


#define SIZE 12
#define MTIME 12
#define TYPEFLAG 1
#define MAGIC 6
#define VERSION 2
#define UNAME 32
#define GNAME 32
#define PREFIX 155
#define FLAG_T 't'
#define FLAG_V 'v'
#define FLAG_F 'f'
#define FLAG_C 'c'

extern int v_flag;
extern int f_flag;

struct headerB {
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
	//char padding[12];	
};


void print_usage_message(char *program_name);
void handle_invalid_flag_selection(char *program_name, char invalid_flag);
int main(int argc, char *argv[]);
void print_archive(int fd, char *argv[], int argc, char flag);
int t_arg(char *file, char type, char *argv[], int argc);
char *path_maker(char *prefix, char *name);


#endif