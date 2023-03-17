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
    // fprintf(stderr, "%s: invalid selection '%c'.\n", 
	// program_name, invalid_flag);
	perror("Invalid flag inputted");
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
		perror("Give f option when passing in file");
		exit(1);
    }
    else {
        // Open the specified file
        fd = open(argv[2], O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(1);
        }

        // Call function to process the archive
        print_archive(fd, argv, argc, flag);
        close(fd);
    }

    return 0;
}


void print_archive_v_flag(struct header *head, char permissions_string[10]) {
    //char rwx_chars[] = {'r', 'w', 'x'};
    // Convert file mode string to integer
    int st_mode = strtol(head->mode, NULL, 8);

    // Modify permissions_string to represent the file's permissions
    int i;

    for (i = 0; i < 10; i++) {
        if (!((st_mode << i) & 01000)) {
            permissions_string[i] = '-';
        }
    }

    // Determine file type and modify permissions_string accordingly
    switch (*(head->typeflag)) {
        case '5': // Directory
            permissions_string[0] = 'd';
            break;
        case '2': // Symbolic link
            permissions_string[0] = 'l';
            break;
        default: // Regular file or other type
            permissions_string[0] = '-';
            break;
    }

    // Convert modification time string to time_t and then to struct tm
    time_t t = strtol(head->mtime, NULL, 8);
    struct tm *time = localtime(&t);

    // Allocate memory for owner_name
    char *owner_name = malloc(strlen(head->uname) + strlen(head->gname) + 2);

    // Concatenate owner's username and groupname to owner_name
    sprintf(owner_name, "%s/%s", head->uname, head->gname);

    // Print formatted file metadata
    printf("%s %-17s %8ld %04d-%02d-%02d %02d:%02d ",
           permissions_string, owner_name, strtol(head->size, NULL, 8),
           time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
           time->tm_hour, time->tm_min);

    // Reset permissions_string to default value
    strcpy(permissions_string, "drwxrwxrwx");

    // Free memory allocated for owner_name
    free(owner_name);
}

void print_archive(int fd, char *argv[], int argc, char curr_flag) {
    // Initialize a variable to keep track of the number of blocks
    int block_counter; 
    int file_size; // Initialize a variable to keep track of the file size
    char *path; // Initialize a pointer to a char to hold the path of a file
    struct header *head = malloc(512); // Allocate memory for a header struct
    // Initialize a string to hold the permissions for the file
    char permissions_string[10] = "drwxrwxrwx"; 
    // Loop through the file until the end of file is reached
    while (0 < read(fd, head, 512)) { 
        // If the name of the header is not present, break out of the loop
        if (!head->name[0]) { 
            break;
        }
        // Concatenate the prefix and name of the file to get the full path
        path = path_concatenate(head->prefix, head->name); 
        // Convert the file size from string to integer
        file_size = strtol(head->size, NULL, 8); 
        /*If the header is not valid, display 
        an error message and exit the program*/
        if (!is_header_valid(head)) { 
			perror("Bad header passed in");
            free(path);
            free(head);
            exit(7);
        }		
        /* Check if the file should be printed 
        based on the arguments passed*/
        if (argc < 4 || (t_arg(path, *head->typeflag, argv, argc) 
        && curr_flag == 't')) { 
            // If the flag is 't', print the file details
            if (curr_flag == 't') { 
                if (v_flag) { /* If the verbose flag is set, print 
                the permissions and other details about the file*/
                    print_archive_v_flag(head, permissions_string);
                }
				// else{ // If the verbose flag is not 
                //set, print only the path of the file
				// 	printf("%s\n", path);
				// }
            }
            if (head->prefix[0]) { // If the prefix of the file is present, 
            //concatenate a slash to the end of the prefix
				strcat(head->prefix, "/");
			}
            // Print the prefix and name of the file
            printf("%.155s%.100s\n", head->prefix, head->name); 
				head->prefix[strlen(head->prefix)] = '\0';

            // If the file size is greater than 0, calculate 
            //the number of blocks and seek to the next block
            if (file_size > 0) { 
                block_counter = (((file_size / 512) + 1) * 512);
                lseek(fd, block_counter, SEEK_CUR);
            }
            // Free the memory allocated for the path  
            free(path);  
        }
        // If the file size is not 0, seek to the 
        //next block and free the memory allocated for the path
        else if (file_size) {              
            lseek(fd, ((file_size / 512) + 1) * 512, SEEK_CUR);
            free(path);
        }        
    }
    // Free the memory allocated for the header
    free(head);
}


/*This function takes a file name, a 
file type, an array of command line arguments, 
and the number of arguments as input parameters.*/
int t_arg(char *file, char type, char *argv[], int argc) {
    int i, j;
    // Iterate through the command line 
    //arguments starting from the fourth argument.
    for (i = 3; i < argc; i++) {
        // Get the current prefix from the command line arguments.
        char* prefix = argv[i];
        // Set the current file path to the file name.
        char* path = file;
        // Iterate through the current prefix and 
        //compare it to the beginning of the file path.
        for (j = 0; j < strlen(prefix); j++) {
            // If there is a mismatch between the prefix
            //and the file path, break out of the loop.
            if (prefix[j] != path[j]) {
                break;
            }
        }
        // If the prefix matches the beginning of the file path and the next 
        /*character in the path is a forward 
        slash or null terminator, return true.*/
        if (j == strlen(prefix) && (path[j] == '/' || path[j] == '\0')) {
            return 1;
        }
    }
    // If no matches are found, return false.
    return 0;
}


/*Function is essentially checking to see if 
the instance of a header encountered is valid*/
int is_header_valid(struct header *head) {
    int check_sum = 0;
    int i;
    for (i = 0; i < 512; i++) {
        if (i < CHKSUM_THRESHOLD  || i >= CHKSUM_THRESHOLD + STRUCT_CHKSUM) {
            check_sum += (unsigned char)((unsigned char*)(head))[i];
        } else {
            check_sum  += ' ';
        }
    }
    if (check_sum != strtol(head->chksum, NULL, 8) ||
        strncmp(head->magic, "ustar", STRUCT_MAGIC - 1) ||
        (0 && (strncmp(head->version, "00", STRUCT_VERSION) ||
        strncmp(head->magic, "ustar\0", STRUCT_MAGIC) ||
        (head->uid[0] < '0' || head->uid[0] > '7')))) {
        return 0;
    }
    return 1;
}

/* This function helps us construct a full path from a header that has its
 * pathname split into the prefix section of the header. */
char *path_concatenate(char *prefix, char *name) {
    int len_prefix = strlen(prefix); 
    int len_name = strlen(name);
    // Allocate memory for the new path string
    // extra chars for null chars and /'s
	char *path = (char*)calloc(len_prefix + len_name + 1,
		+300);
    // Copy prefix to new path string, if prefix is not empty
	if (prefix[0]) {
		strcpy(path, prefix);
		strcat(path, "/");
	}
    // Concatenate name to new path string
	strcat(path, name);
	strcat(path, "\0");
	return path;
}
