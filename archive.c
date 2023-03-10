#include "archive.h"

struct dirent *direnty;
struct passwd *password;
struct group *group;
struct header *headerer;

void populate_header(stat_ptr sp){
    /*accepts a pointer to a stat struct
    populates the global header struct with
    data from the passed stat struct*/
    memset(&header, 0, sizeof(header)); // clear header struct
    strcpy(header->name, ""); // copy empty string to name field
    snprintf(header->mode, 8, "%07o", sp->st_mode & 0777); // convert mode to octal and copy to mode field
    snprintf(header->uid, 8, "%07o", sp->st_uid); // convert uid to octal and copy to uid field
    snprintf(header->gid, 8, "%07o", sp->st_gid); // convert gid to octal and copy to gid field
    snprintf(header->size, 12, "%011llo", (unsigned long long) sp->st_size); // convert size to octal and copy to size field
    snprintf(header->mtime, 12, "%011lo", (unsigned long) sp->st_mtime); // convert mtime to octal and copy to mtime field
    header->typeflag = '0'; // set typeflag to regular file
    strcpy(header->linkname, ""); // copy empty string to linkname field
    strcpy(header->magic, "ustar"); // copy "ustar" to magic field
    strcpy(header->version, "00"); // copy "00" to version field
    password = getpwuid(sp->st_uid);
    strcpy(header->uname, password->pw_name); // copy empty string to uname field
    group = getgrgid(sp->st_gid);
    strcpy(header->gname, group->gr_name); // copy empty string to gname field
    strcpy(header->devmajor, ""); // copy empty string to devmajor field
    strcpy(header->devminor, ""); // copy empty string to devminor field
    strcpy(header->prefix, ""); // copy empty string to prefix field
    memset(header->padding, '\0', 52); // fill padding field with null characters
}

void populate_header_buffer(headerer_ptr hp){
    /*take a header_ptr
    populates the global buffer with the elements of the header*/
    memcpy(mem_block, hp, sizeof(header)); // copy header struct to mem_block   
}

void manage_file(FILE file){
    /*takes file
    if directory, archive and expand directory,
    if file, archive file*/
    struct stat *sb;
    if (fstat(fileno(file), sb) == -1) { // get file information
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    if (S_ISDIR(sb->st_mode)) { // if file is a directory
        expand_directory(file);
    } else { // if file is a regular file
        archive_file(file);
    }
};


void archive_file(FILE file){
    /*accepts file,
    calls populate_header/buffer functions
    writes body to buffer*/
    struct stat sb;
    if (fstat(fileno(file), &sb) == -1) { // get file information
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    populate_header(&sb); // populate header with file information
    populate_header_buffer(&header); // populate buffer with header information

    size_t remaining_bytes = sb.st_size; // track remaining bytes to read

    while (remaining_bytes > 0) {
        int bytes_to_read = remaining_bytes > sizeof(mem_block) ? sizeof(mem_block) : remaining_bytes;
        size_t bytes_read = fread(mem_block, 1, bytes_to_read, file); // read bytes from file
        if (bytes_read != bytes_to_read) {
            perror("fread")
        }
    }
}

void expand_directory(FILE file){
    /*accepts file,
    for each child file in dir, call manage file*/
    ;
}

void array_to_buffer(char* arr){
    /*accepts a char*
    takes each element of char* and puts it in the global buffer*/
    ;
}

void write_to_buffer(int ch){
    /*Takes a char as an int (inorder to accept -1 and have 0-255 for char)
    and writes it to buffer. Static buffer index, if index == 512, 
    write buffer to global out file, set index to 0. If FORCE (-1)
    marco is passed fill remainder of buffer with '\0', write to global
    outfile, and set index to to 0*/
    ;
}



void decToOctal(int n){
    // array to store octal number
    int octalNum[100];
  
    // counter for octal number array
    int i = 0;
    while (n != 0) {
  
        // storing remainder in octal array
        octalNum[i] = n % 8;
        n = n / 8;
        i++;
    }
  
    // printing octal number array in reverse order
    for (int j = i - 1; j >= 0; j--)
        printf("%d", octalNum[j]);
}
  

