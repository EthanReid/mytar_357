#include "archive.h"
#include "mytar.h"
#include "blockBuffer.h"
header head;

void populate_header(char *name, stat_ptr sp){
    /*use snprintf to move stat data to head*/

}


void write_header(){
    /*memcpy head into block, if head has packed attribute,
    could also call write_byte in a loop, where each byte of head is moved to bock.
    write_block after filling if using memcpy*/
    ;
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
            write_block();
        }
    }
    fclose(file);
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
    dir_name = concat_str(name, "/");
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
    free(dir_name);
}

void array_to_buffer(char* arr){
    /*accepts a char*
    takes each element of char* and puts it in the global buffer,
    dont need if doing memcpy*/
    ;
}


/*snprintf*/
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

char *concat_str(char *str1, char *str2){
    char *c_str;
    c_str = calloc(1, strlen(str1)+strlen(2));
    snprintf(c_str, sizeof(c_str),"%s%s",str1, str2);
}
  

