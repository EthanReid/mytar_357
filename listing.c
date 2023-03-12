#include "listing.h"
#include "mytar.h"
#include "archive.h"
#include "blockBuffer.h"

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



void list_archives(v_flag){
    /* Function will list contents of a tar file in stdout if passed 
    a verbose flag as the third argument, it will provide additional 
    information on permissions corresponding to the file*/
    struct tm *time;
    char permissions[10];
    char owner_name[17];
    char file_size[8];
    char m_time_buffer[16]; 

    int file_size;
    file_size = strtol(header->size, NULL, 8);

    int w_r_x_permissions;
    w_r_x_permissions = strtol(header->mode, NULL, 8);



    time = localtime(&m_time);

    //m_time = octal_to_string(header->mtime);
    //file_name is variable 

    if (!(v_flag)){
        printf("%s\n", file);
    } 
    //handling permissions byte below for verbose flag
    else if(v_flag){
        //don't need to check if v-flag since only other poss
        if (header->typeflag == SYM_LINK) {
            permissions[0] = '1';
        }
        elif (header->typeflag == REGULAR_FILE) {
            permissions[0] = '-';
        } 
        else (header->typeflag == DIRECTORY) {
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
        if (w_r_x_permissions` & S_IXOTH) {
            permissions[9] = 'x';
        } 
        else {
            permissions[9] = '-';
        }
        if (strlen(head.uname)>17){
            strncopy(owner_name, head.uname, 17);
        }
        else{
            //compute how
            int len_gname = (17 - (strlen(head.uname)));
            //add "/" between owner and group
            strcat(owner_name, "/");
            //account for "/"
            len_gname -= 1;
            //add as much of gname as space is left 
            strncopy(owner_name, head.gname, len_gname);
        }

        //inserting time attributes into char buffer
        sprintf(m_time_buffer, "%04i-%02i-%02i %02i:%02i",
            1900 + time->tm_year,
            time->tm_mon + 1,
            time->tm_mday,
            time->tm_hour,
            time->tm_min);

        printf("%10s %-17s %8i %16s %s\n", permissions, owner_name, file_size, m_time_buffer, file_name);
    }
}
        
//archive = file pointer declared in mytar.c   

int print_archive(in_file, v_flag) {
    //open file passed in
    
    //global header pointer header_ptr declared in archive.h
    int i;
    int result;
    //will come from concatting name & pre-fix after reading in struct
    char file_name[257];
    char *name_offset;

    while ((fread(header_ptr, 512, 1, in_file))!=0 ){
        int name_len = strlen(header_ptr->name);
        int prefix_len = strlen(header_ptr->prefix);
        //case where prefix does not exist 
        //will not be concatenated to the name
        if strlen(header_ptr->prefix) == 0{ 
            strncpy(file_name, header_ptr->name, name_len);      
        }
        else{
            //case where pre-fix is not empty
            strncpy(file_name, header_ptr->prefix, prefix_len);
            file_name[prefix_len] = '/';
            name_offset = &file_name + prefix_len + 1;
            strncpy(name_offset, header_ptr->name, strlen(name));
            
        }
    }



    /* Loop rad the each header */
    while (read_header(archive, header) != 0) {
        /* Create a char of the name of the file */
        create_name(file_name, header->prefix, header->name);

        /* If no files are given, print the whole archive */
        if (argc < 4) {
            list_archives(file_name, header, verbose_flag);
        } else {
            /* If files are given, find each one in the order given */
            for (i = 3; i < argc; i++) {
                /* Check if current file is the same as the file passed */
                if (check_file(file_name, argv[i]) == 0) {
                    /* If it is, print */
                    list_archives(file_name, header, verbose_flag);
                }
            }
        }
    }
    /* Close oopened files */
    fclose(archive);
    return result;
}
        


        


        