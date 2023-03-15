#include "listing2.h"

FILE *in_file;

int v_flag = 0;
int f_flag = 0;


// Define function to print usage message
void print_usage_message(char *program_name) {
    fprintf(stderr, "%s: Must pass option for operation desired "
                    "options.\n", program_name);
    fprintf(stderr, "usage: %s [ctxSp[f tarfile]] "
                    "[file1 [ file2 [...] ] ]\n", program_name);
}

// Define function to handle invalid flag selection
void handle_invalid_flag_selection(char *program_name, char invalid_flag) {
    fprintf(stderr, "%s: invalid selection '%c'.\n", 
	program_name, invalid_flag);
}

int main(int argc, char *argv[]) {
    // Declare and initialize variables
    int fd = -1;
    int flag = 0;
    int ct = 0;
	int i;
    // Check if user passed in enough arguments
    if (argc < 2) {
        print_usage_message(argv[0]);
        return 1;
    }

    // Parse command-line arguments
    for (i = 0; i < strlen(argv[1]); i++) {
        char current_flag = argv[1][i];
        switch (current_flag) {
            case FLAG_T:
                ct++;
                if (!flag) {
                    flag = FLAG_T;
                }
                break;
            case FLAG_V:
                v_flag = 1;
                break;
            case FLAG_F:
                f_flag = 1;
                break;
            default:
                handle_invalid_flag_selection(argv[0], current_flag);
        }
    }   

    // Check if f flag was passed in
    if (!f_flag) {
        fprintf(stderr, "%s: give f option if passing in file\n", argv[0]);
        return 1;
    }
    else {
        // Open the specified file
        fd = open(argv[2], O_RDONLY);
        if (fd == -1) {
            perror("open");
            return 1;
        }

        // Call function to process the archive
        print_archive(fd, argv, argc, flag);
        close(fd);
    }

    return 0;
}



void print_archive(int fd, char *argv[], int argc, char flag) {
	int file_size, i, st_mode, chksum;
	char *owner_name;
	int block_counter;
	char *path;
	struct header *head = (struct header*)malloc(sizeof(struct header));
	time_t t;
	struct tm *time;
	char perms[10] = "drwxrwxrwx";
	while (0 < read(fd, head, 512)) {
		if (!head->name[0]) {
			break;
		}
		path = path_maker(head->prefix, head->name);
		file_size = strtol(head->size, NULL, 8);
		if (argc < 4 || (t_arg(path, *head->typeflag, argv, argc) &&
			flag == 't')) {
			chksum = 0;
			for (i = 0; i < 512; i++) {
				if (i < CHKSUM_OFF ||
					i >= CHKSUM_OFF + CHKSUM) {
					chksum += (unsigned char)
						((unsigned char*)(head))[i];
				}
				else {
					chksum += ' ';
				}
			}
			//bad header check
		if (chksum != strtol(head->chksum, NULL, 8)
			|| strncmp(head->magic, "ustar", MAGIC - 1) ||
			(0 && (strncmp(head->version, "00",
			VERSION) || strncmp(head->magic, "ustar\0",
			MAGIC) || (head->uid[0] < '0' ||
			head->uid[0] > '7')))) {
			fprintf(stderr, "Malformed header found.  "
				"Bailing.\n");
			free(path);
			free(head);
			exit(7);
		}
			//generalcase for flag t being set 
		if (flag == 't') {
			//handling sub_case of the v_flag
			if (v_flag) {
				st_mode = strtol(head->mode, NULL, 8);
				for (i = 0; i < 10; i++) {
					if (!((st_mode << i) & 01000)) {
						perms[i] = '-';
					}
					}
					if (*(head->typeflag) == '5') {
						perms[0] = 'd';
					}
					else if (*(head->typeflag) == '2') {
						perms[0] = 'l';
					}
					else {
						perms[0] = '-';
					}
					t = strtol(head->mtime, NULL, 8);
					time = localtime(&t);
					owner_name = (char*)malloc(sizeof(char)*
						(strlen(head->uname) +
						strlen(head->gname) + 2));
					strcpy(owner_name, head->uname);
					strcat(owner_name, "/");
					strcat(owner_name, head->gname);
					printf("%s %-17s %8ld %d-%02d-%02d"
					       " %02d:%02d ",
						perms, owner_name,
						strtol(head->size, NULL, 8),
						1900 + time->tm_year,
						1 + time->tm_mon,
						time->tm_mday, time->tm_hour,
						time->tm_min);
					strcpy(perms, "drwxrwxrwx");
					free(owner_name);
				}
				if (head->prefix[0]) {
					strcat(head->prefix, "/");
				}
				printf("%.155s%.100s\n", head->prefix,
					head->name);
				head->prefix[strlen(head->prefix)] = '\0';
				if (file_size > 0) {
				 	block_counter = 
					(((file_size / 512) + 1) * 512);
					lseek(fd, block_counter, SEEK_CUR);
				}
				free(path);
			}

		}
		//case for the t option being selected
		else if (file_size) {
			lseek(fd, ((file_size / 512) + 1) * 512, SEEK_CUR);
			free(path);
		}
	}
	free(head);
}



// void print_archive(int fd, char *argv[], 
// int argc, char flag) {
//     int file_size, padding, i;
//     char *file_path;
//     struct header *header = (struct header*)malloc(sizeof(struct header));
//     while (0 < read(fd, header, head)) {
//         if (!header->name[0]) {
//             break;
//         }
//         file_path = path_maker(header->prefix, header->name);
//         file_size = strtol(header->size, NULL, OCTAL);
//         if (argc < 4 || (tin(file_path, *header->typeflag, 
// 		argv, argc) && flag == 't')) {
//             if (is_valid_header(header, S_flag)) {
//                 if (flag == 't') {
//                     print_file(fd, header, v_flag, argv, argc);
//                 }
//             } else {
//                 fprintf(stderr, "Malformed header found. Bailing.\n");
//                 free(file_path);
//                 free(header);
//                 exit(7);
//             }
//             if (file_size > 0) {
//                 padding = (((file_size / head) + 1) * head) - file_size;
//                 skip_file(fd, padding);
//             }
//             free(file_path);
//         } else if (file_size) {
//             skip_file(fd, ((file_size / head) + 1) * head);
//             free(file_path);
//         }
//     }
//     free(header);
// }

// int is_valid_header(struct header *header, char S_flag) {
//     int checksum, i;
//     checksum = 0;
//     for (i = 0; i < head; i++) {
//         if (i < CHKSUM_OFF || i >= CHKSUM_OFF + CHKSUM) {
//             checksum += (unsigned char)((unsigned char*)(header))[i];
//         } else {
//             checksum += ' ';
//         }
//     }
//     return (checksum == strtol(header->chksum, NULL, OCTAL)
//         && strncmp(header->magic, "ustar", MAGIC - 1)
//         && (!S_flag || (strncmp(header->version, "00", VERSION)
//         || strncmp(header->magic, "ustar\0", MAGIC)
//         || (header->uid[0] < '0' || header->uid[0] > '7'))));
// }


       
// void print_permissions(mode_t mode, char *permissions) {
//     permissions[0] = '-';
//     if (S_ISDIR(mode)) {
//         permissions[0] = 'd';
//     } else if (S_ISLNK(mode)) {
//         permissions[0] = 'l';
//     }
//     if (mode & S_IRUSR) {
//         permissions[1] = 'r';
//     }
//     if (mode & S_IWUSR) {
//         permissions[2] = 'w';
//     }
//     if (mode & S_IXUSR) {
//         permissions[3] = 'x';
//     }
//     if (mode & S_IRGRP) {
//         permissions[4] = 'r';
//     }
//     if (mode & S_IWGRP) {
//         permissions[5] = 'w';
//     }
//     if (mode & S_IXGRP) {
//         permissions[6] = 'x';
//     }
//     if (mode & S_IROTH) {
//         permissions[7] = 'r';
//     }
//     if (mode & S_IWOTH) {
//         permissions[8] = 'w';
//     }
//     if (mode & S_IXOTH) {
//         permissions[9] = 'x';
//     }
//     permissions[10] = '\0';
// }


// void print_file(int fd, struct header *header, 
// char flag, char *argv[], int argc) {
//     int fdout;
//     char *path;
//     int fsize = strtol(header->size, NULL, OCTAL);

//     if (*(header->typeflag) == '5') {
//         // directory
//         if (flag == 't') {
//             printf("%s\n", header->name);
//         } else {
//             printf("x %s\n", header->name);
//         }
//     } else {
//         // regular file
//         if (flag == 't') {
//             printf("%s\n", header->name);
//         } else {
//             printf("x %s\n", header->name);
//             path = path_maker(header->prefix, header->name);
//             fdout = creat(path, 0666);
//             free(path);
//             if (fdout < 0) {
//                 perror("creat");
//                 exit(1);
//             }
//             copy(fd, fdout, fsize);
//             close(fdout);
//             if (chmod(header->name, strtol(header->mode, NULL, 8)) < 0) {
//                 perror("chmod");
//                 exit(1);
//             }
//             if (chown(header->name, strtol(header->uid, NULL, 8), 
// 			strtol(header->gid, NULL, 8)) < 0) {
//                 perror("chown");
//                 exit(1);
//             }
//         }
//     }
// }

// void skip_file(int fd, int fsize) {
//     int n;
//     char buf[BUFSIZ];

//     while (fsize > 0) {
//         n = (fsize < BUFSIZ) ? fsize : BUFSIZ;
//         if (read(fd, buf, n) != n) {
//             perror("read");
//             exit(1);
//         }
//         fsize -= n;
//     }
// }

int t_arg(char *file, char type, char *argv[], int argc) {
    int i, j;
    for (i = 3; i < argc; i++) {
        char* prefix = argv[i];
        char* path = file;
        for (j = 0; j < strlen(prefix); j++) {
            if (prefix[j] != path[j]) {
                break;
            }
        }
        if (j == strlen(prefix) && (path[j] == '/' || path[j] == '\0')) {
            return 1;
        }
    }
    return 0;
}


/* This function helps us construct a full path from a header that has its
 * pathname split into the prefix section of the header. */
char *path_maker(char *prefix, char *name) {
	char *path = (char*)calloc(strlen(prefix) + strlen(name) + 1,
		sizeof(char));
	if (prefix[0]) {
		strcpy(path, prefix);
		strcat(path, "/");
	}
	strcat(path, name);
	return path;
}

