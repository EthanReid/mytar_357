#include "listing.h"
#include "archive.h"
//#include "archive.c"

//#include "mytar.h"
//#include "mytar.h"
//#include "archive.h"
//#include "blockBuffer.h"

/*
In list (Table of contents) mode, mytar lists the contents of the given archive file, 
in order, one per line. If no names are given on the command line, mytar, 
lists all the files in the archive. If a name or names are given on the command line,
mytar will list the given path and any and all descendents of it. 
That is, all files and directories beginning with the same series of directories.
If the verbose (’v’) option is set, mytar gives expanded information 
about each file as it lists them.
*/


//must read the file into the global header 

//header head;

void list_archives(char *file_name, int v_flag, header *head){
    /* Function will list contents of a tar file in stdout if passed 
    a verbose flag as the third argument, it will provide additional 
    information on permissions corresponding to the file*/
    struct tm *time = NULL;

    char permissions[10];
    char owner_name[17];
    //char file_size_buffer[8];
    char m_time_buffer[16]; 

    int file_size;
    file_size = strtol(head->size, NULL, 8);

    int w_r_x_permissions;
    w_r_x_permissions = strtol(head->mode, NULL, 8);

    time_t mtime = strtol(head->mtime, NULL, 8);
    //octal_to_string(header->mtime);

    time = localtime(&mtime);

    //m_time = octal_to_string(header->mtime);
    //file_name is variable 

    if (!(v_flag)){
        printf("%s\n", file_name);
    } 
    //handling permissions byte below for verbose flag
    else if(v_flag){
        //don't need to check if v-flag since only other poss
        if (head->typeflag == SYM_LINK){
            permissions[0] = '1';
        }
        else if (head->typeflag == REGULAR_FILE){
            permissions[0] = '-';
        } 
        else if (head->typeflag == DIRECTORY){
            permissions[0] = 'd';
        }
        //must populate rest of the array with permissions info
                /* Find the 9 bytes of permisions for mode */
        if (w_r_x_permissions & S_IRUSR){
            permissions[1] = 'r';
        } 
        else {
            permissions[1] = '-';
        }
        if (w_r_x_permissions & S_IWUSR){
            permissions[2] = 'w';
        } 
        else {
            permissions[2] = '-';
        }   
        if (w_r_x_permissions & S_IXUSR) {
            permissions[3] = 'x';
        } 
        else {
            permissions[3] = '-';
        }
        if (w_r_x_permissions & S_IRGRP) {
            permissions[4] = 'r';
        } 
        else {
            permissions[4] = '-';
        }
        if (w_r_x_permissions & S_IWGRP) {
            permissions[5] = 'w';
        } 
        else {
            permissions[5] = '-';
        }
        if (w_r_x_permissions & S_IXGRP) {
            permissions[6] = 'x';
        } 
        else {
            permissions[6] = '-';
        }
        if (w_r_x_permissions & S_IROTH) {
            permissions[7] = 'r';
        } 
        else {
            permissions[7] = '-';
        }
        if (w_r_x_permissions & S_IWOTH) {
            permissions[8] = 'w';
        } 
        else {
            permissions[8] = '-';
        }
        if (w_r_x_permissions & S_IXOTH) {
            permissions[9] = 'x';
        } 
        else {
            permissions[9] = '-';
        }
        if (strlen(head->uname)>17){
            strncpy(owner_name, head->uname, 17);
        }
        else{
            //compute how
            int len_gname = (17 - (strlen(head->uname)));
            //add "/" between owner and group
            strcat(owner_name, "/");
            //account for "/"
            len_gname -= 1;
            //add as much of gname as space is left 
            strncpy(owner_name, head->gname, len_gname);
        }

        //inserting time attributes into char buffer
        sprintf(m_time_buffer, "%04i-%02i-%02i %02i:%02i",
            1900 + time->tm_year,
            time->tm_mon + 1,
            time->tm_mday,
            time->tm_hour,
            time->tm_min);

        printf("%s %-17s %8i %16s %s\n", head->uname, owner_name, file_size, m_time_buffer, file_name);
    }
    return;
}
        

//archive = file pointer declared in mytar.c   

int check_file(char *file, char *compare) {
    char file_buffer[strlen(compare) + 1];
    char compare_buffer[strlen(compare) + 1];
    
    /* sets the two buffers to all '\0' */
    memset(file_buffer, '\0', strlen(compare) + 1);
    memset(compare_buffer, '\0', strlen(compare) + 1);
    /* Copies the file name into the file buffer with the same number of characters as its compared to name */
    strncpy(file_buffer, file, strlen(compare));
    /* Copies the compared to name to the compared buffer */
    strcpy(compare_buffer, compare);
    /* return the compared result */
    return strcmp(file_buffer, compare_buffer);
}

int print_archive(FILE *in_file, int v_flag, int argc, char **argv) {
    /*Function will get name of tar file passed in, will be passed along to
    list_archives()*/
    //open file passed in
    //
    header_ptr head = malloc(sizeof(header));
    int i;
    int result;
    //will come from concatting name & pre-fix after reading in struct
    //char *file_name[257];
    //declared file_name as a global array
    char *name_offset;

    //while reading the header into the struct is possible
    //1 determines the number of reads to do
    while ((fread(head, sizeof(header), 1, in_file))!=0 ){
        int name_len = strlen(head->name);
        int prefix_len = strlen(head->prefix);
        printf("%d \n", name_len);
        //case where prefix does not exist will not be concatenated to the name
        //generating file name without pre-fix 
        if (strlen(head->prefix) == 0){ 
            strncpy(file_name, head->name, prefix_len);      
        }
        else if (prefix_len != 0){
            //case where pre-fix is not empty
            strcat(file_name, head->prefix);
            strcat(file_name, "/");
            strcat(file_name, head->name);
            file_name[strlen(file_name)+1] = '\0';
            // name_offset = file_name + prefix_len + 1;
            // strncpy(name_offset, head.prefix, prefix_len);

            // strncpy(file_name, head.name, name_len);
            // file_name[name_len] = '/';
            // name_offset = file_name + prefix_len + 1;
            // strncpy(name_offset, head.prefix, prefix_len);
        }
    }
        /* If no files are given, print the whole archive */
        if (argc < 4) {
            list_archives(file_name, 1, head);
        } else {
             //for file case ensuring we are indexing above other args passed in
            for (i = 3; i < argc; i++) {
                /* Check if current file is the same as the file passed */
                if (check_file(file_name, argv[i]) == 0) {
                    /* If it is, print */
                    list_archives(file_name, v_flag, head);
                }
            }
        }
    
    strcpy(head->uname, "test1212\0");
    /* Close opened files */
    fclose(in_file);
    return 0;

}
        


        


        