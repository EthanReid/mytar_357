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
  

