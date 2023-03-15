#include "archive.h"
#include "blockBuffer.h"

header head;

void populate_header(char *name, stat_ptr sp){
    struct passwd *password;
    struct group *group;
    /*use snprintf to move stat data to head*/
    memset(&head, 0, sizeof(header)); // clear header struct

    snprintf(head.mode, 8, "%07o", sp->st_mode & 0777); //why 0077?
    // convert uid to octal and copy to uid field
    snprintf(head.uid, 8, "%07o", sp->st_uid); 
     // convert gid to octal and copy to gid field
    snprintf(head.gid, 8, "%07o", sp->st_gid);
    //strcpy(head.uid, "7777777");
    //strcpy(head.gid, "7777777");
     // doesnt exist?
    snprintf(head.mtime, 12, "%011lo", (unsigned long) sp->st_mtime);
    head.typeflag = '0'; // set typeflag to regular file
    strcpy(head.linkname, ""); // copy empty string to linkname field
    strcpy(head.magic, "ustar"); // copy "ustar" to magic field
    head.version[0] = '0';
    head.version[1] = '0';
    password = getpwuid(sp->st_uid); 
    // copy empty string to uname field
    strcpy(head.uname, password->pw_name); 
    group = getgrgid(sp->st_gid);
    // copy empty string to gname field
    strcpy(head.gname, group->gr_name); 
    strcpy(head.devmajor, ""); // copy empty string to devmajor field
    strcpy(head.devminor, ""); // copy empty string to devminor field
    
    //copy first 100 chars of name
    if (strlen(name)>100){
        strncpy(head.name, name, 100);
         //copy from element 100 until end
        strncpy(head.prefix, (name+100),strlen(name)-100);
    }else{
        strncpy(head.name, name, strlen(name)%101);
    }

    if (S_ISREG(sp->st_mode)){
        head.typeflag = '0';
         // convert size to octal and copy to size field
        snprintf(head.size, 12, "%011llo", (unsigned long long) sp->st_size);
    }else if (S_ISLNK(sp->st_mode)){
        head.typeflag = '2';
        strcpy(head.size, "00000000000");
    }else{
        head.typeflag = '5';
        strcpy(head.size, "00000000000");
    }

    int chksum = 0;
    int i;
    for (i = 0; i<BLOCK_SIZE; ++i){
        chksum += ((char*)&head)[i];
    }
    chksum += ' ' * 8;

    snprintf(head.chksum, 8, "%07o", chksum);
}


//previously populate_header_buffer
void write_header(){
    /*memcpy head into block, if head has packed attribute,
    could also call write_byte in a loop, 
    where each byte of head is moved to bock.
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
            manage_file(path_name);
            free(path_name);
        }
    }
    //free(dir_name);
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
