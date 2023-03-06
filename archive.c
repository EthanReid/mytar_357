#include "archive.h"

void populate_header(stat_ptr sp){
    /*accepts a pointer to a stat struct
    populates the global header struct with
    data from the passed stat struct*/
}


void populate_header_buffer(headerer_ptr hp){
    /*take a header_ptr
    populates the glabal buffer with the elements of the header*/
    ;
}

void manage_file(FILE file){
    /*takes file
    if directory, archive and expand directory,
    if file, archive file*/
    ;
}

void archive_file(FILE file){
    /*accepts file,
    calls populate_header/buffer functions
    writes body to buffer*/
    ;
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
  

