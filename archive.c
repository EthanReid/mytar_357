#include "archive.h"
#include "blockBuffer.h"

//#include "mytar.h"
//#include "blockBuffer.h"

header head;
// void populate_header(stat_ptr sp){
//     /*accepts a pointer to a stat struct
//     populates the global header struct with
//     data from the passed stat struct*/
//     memset(&header, 0, sizeof(header)); // clear header struct
//     strcpy(header->name, ""); // copy empty string to name field
//     snprintf(header->mode, 8, "%07o", sp->st_mode & 0777); // convert mode to octal and copy to mode field
//     snprintf(header->uid, 8, "%07o", sp->st_uid); // convert uid to octal and copy to uid field
//     snprintf(header->gid, 8, "%07o", sp->st_gid); // convert gid to octal and copy to gid field
//     snprintf(header->size, 12, "%011llo", (unsigned long long) sp->st_size); // convert size to octal and copy to size field
//     snprintf(header->mtime, 12, "%011lo", (unsigned long) sp->st_mtime); // convert mtime to octal and copy to mtime field
//     header->typeflag = '0'; // set typeflag to regular file
//     strcpy(header->linkname, ""); // copy empty string to linkname field
//     strcpy(header->magic, "ustar"); // copy "ustar" to magic field
//     strcpy(header->version, "00"); // copy "00" to version field
//     password = getpwuid(sp->st_uid);
//     strcpy(header->uname, password->pw_name); // copy empty string to uname field
//     group = getgrgid(sp->st_gid);
//     strcpy(header->gname, group->gr_name); // copy empty string to gname field
//     strcpy(header->devmajor, ""); // copy empty string to devmajor field
//     strcpy(header->devminor, ""); // copy empty string to devminor field
//     strcpy(header->prefix, ""); // copy empty string to prefix field
//     memset(header->padding, '\0', 52); // fill padding field with null characters
// }


void populate_header(char *name, stat_ptr sp){
    struct passwd *password;
    struct group *group;
    /*use snprintf to move stat data to head*/
    memset(&head, 0, sizeof(header)); // clear header struct
    snprintf(head.mode, 8, "%07o", sp->st_mode & 0777); //why 0077?
    snprintf(head.uid, 8, "%07o", sp->st_uid); // convert uid to octal and copy to uid field
    snprintf(head.gid, 8, "%07o", sp->st_gid); // convert gid to octal and copy to gid field
    snprintf(head.size, 12, "%011llo", (unsigned long long) sp->st_size); // convert size to octal and copy to size field
    snprintf(head.mtime, 12, "%011lo", (unsigned long) sp->st_mtime); // doesnt exist?
    head.typeflag = '0'; // set typeflag to regular file
    strcpy(head.linkname, ""); // copy empty string to linkname field
    strcpy(head.magic, "ustar"); // copy "ustar" to magic field
    //strcpy(head.version, "00"); // copy "00" to version field - BUG HERE, TRACE TRAP
    head.version[0] = '0';
    head.version[1] = '0';
    password = getpwuid(sp->st_uid); 
    strcpy(head.uname, password->pw_name); // copy empty string to uname field
    group = getgrgid(sp->st_gid);
    strcpy(head.gname, group->gr_name); // copy empty string to gname field
    strcpy(head.devmajor, ""); // copy empty string to devmajor field
    strcpy(head.devminor, ""); // copy empty string to devminor field
    
    //copy first 100 chars of name
    if (strlen(name)>100){
        strncpy(head.name, name, 100);
        strncpy(head.prefix, (name+100),strlen(name)-100); //copy from element 100 until end
    }else{
        strncpy(head.name, name, strlen(name)%101);
    }
}


//previously populate_header_buffer
void write_header(){
    /*memcpy head into block, if head has packed attribute,
    could also call write_byte in a loop, where each byte of head is moved to bock.
    write_block after filling if using memcpy*/
    memcpy(&block, &head, sizeof(head));
    write_block(FORCE);
}


void manage_file(char *name){
    /*takes name
    if directory, archive and expand directory,
    if file, archive file*/
    struct stat st;
    stat_ptr sp = &st;
    lstat(name, sp);
    archive_file(name, sp);
    if (S_ISDIR(sp->st_mode)) expand_directory(name);
} 

//force macro implemented
void archive_file(char *name, stat_ptr sp){
    /*accepts name and stat,
    calls populate_header/buffer functions
    writes body to buffer*/
    FILE *file;
    populate_header(name, sp);
    write_header();
    if (S_ISREG(sp->st_mode)){
        printf("IS FILE\n");
        file = fopen(name, "rb");
        if (file==NULL){
            perror("connot open file");
            exit(EXIT_FAILURE);
        }
        while(fread(&block, BLOCK_SIZE, 1, file)){
            write_block(FORCE);
        }
        write_block(FORCE);
         fclose(file);
    }
}

void expand_directory(char *name){
    /*accepts name,
    for each child file in dir, call manage file*/
    DIR *directory;
    struct dirent *dirp;
    struct stat cur_dir;
    struct stat dir;
    char *dir_name;
    char *path_name;
    lstat(name, &cur_dir);
    //dir_name = concat_str(name, "/");
    dir_name = name;
    if ((directory = opendir(dir_name))==NULL) exit(EXIT_FAILURE);
    while((dirp = readdir(directory))!=NULL){
        /*get ino from name from dirent*/
        stat(dirp->d_name,&dir);
        /*if ino matches passed ino (target)*/
        if ((dirp->d_name[0]!='.')&&(dir.st_ino != cur_dir.st_ino)){
            path_name = concat_str(name, dirp->d_name);
            printf("%s\n",path_name);
            manage_file(path_name);
            free(path_name);
        }
    }
    //free(dir_name);
}

void array_to_buffer(char* arr){
    /*accepts a char*
    takes each element of char* and puts it in the global buffer,
    dont need if doing memcpy*/
    ;
}


/*snprintf*/
//decToOctal function not needed
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

//why not use strcar instead of this method?
char *concat_str(char *str1, char *str2){
    char *c_str;
    int size;
    size = strlen(str1)+strlen(str2)+2;
    if (size>PATH_MAX){
        perror("max path len");
        exit(EXIT_FAILURE);
    }
    c_str = calloc(1, size);
    snprintf(c_str, size,"%s/%s",str1, str2);
    return c_str;
}

// #include "archive.h"

// struct dirent *direnty;
// struct passwd *password;
// struct group *group;
// struct header *headerer;

// void populate_header(stat_ptr sp){
//     /*accepts a pointer to a stat struct
//     populates the global header struct with
//     data from the passed stat struct*/
//     memset(&header, 0, sizeof(header)); // clear header struct
//     strcpy(header->name, ""); // copy empty string to name field
//     snprintf(header->mode, 8, "%07o", sp->st_mode & 0777); // convert mode to octal and copy to mode field
//     snprintf(header->uid, 8, "%07o", sp->st_uid); // convert uid to octal and copy to uid field
//     snprintf(header->gid, 8, "%07o", sp->st_gid); // convert gid to octal and copy to gid field
//     snprintf(header->size, 12, "%011llo", (unsigned long long) sp->st_size); // convert size to octal and copy to size field
//     snprintf(header->mtime, 12, "%011lo", (unsigned long) sp->st_mtime); // convert mtime to octal and copy to mtime field
//     header->typeflag = '0'; // set typeflag to regular file
//     strcpy(header->linkname, ""); // copy empty string to linkname field
//     strcpy(header->magic, "ustar"); // copy "ustar" to magic field
//     strcpy(header->version, "00"); // copy "00" to version field
//     password = getpwuid(sp->st_uid);
//     strcpy(header->uname, password->pw_name); // copy empty string to uname field
//     group = getgrgid(sp->st_gid);
//     strcpy(header->gname, group->gr_name); // copy empty string to gname field
//     strcpy(header->devmajor, ""); // copy empty string to devmajor field
//     strcpy(header->devminor, ""); // copy empty string to devminor field
//     strcpy(header->prefix, ""); // copy empty string to prefix field
//     memset(header->padding, '\0', 52); // fill padding field with null characters
// }

// void populate_header_buffer(headerer_ptr hp){
//     /*take a header_ptr
//     populates the global buffer with the elements of the header*/
//     memcpy(mem_block, hp, sizeof(header)); // copy header struct to mem_block   
// }

// void archive_file(FILE file){
//     /*accepts file,
//     calls populate_header/buffer functions
//     writes body to buffer*/
//     struct stat sb;
//     if (fstat(fileno(file), &sb) == -1) { // get file information
//         perror("fstat");
//         exit(EXIT_FAILURE);
//     }

//     populate_header(&sb); // populate header with file information
//     populate_header_buffer(&header); // populate buffer with header information

//     size_t remaining_bytes = sb.st_size; // track remaining bytes to read

//     while (remaining_bytes > 0) {
//         int bytes_to_read = remaining_bytes > sizeof(mem_block) ? sizeof(mem_block) : remaining_bytes;
//         size_t bytes_read = fread(mem_block, 1, bytes_to_read, file); // read bytes from file
//         if (bytes_read != bytes_to_read) {
//             perror("fread")
//         }
//     }
// }

// void expand_directory(FILE file){
//     /*accepts file,
//     for each child file in dir, call manage file*/
//     ;
// }

// void array_to_buffer(char* arr){
//     /*accepts a char*
//     takes each element of char* and puts it in the global buffer*/
//     ;
// }

// void write_to_buffer(int ch){
//     /*Takes a char as an int (inorder to accept -1 and have 0-255 for char)
//     and writes it to buffer. Static buffer index, if index == 512, 
//     write buffer to global out file, set index to 0. If FORCE (-1)
//     marco is passed fill remainder of buffer with '\0', write to global
//     outfile, and set index to to 0*/
//     ;
// }




