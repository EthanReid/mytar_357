#include "mytar.h"
//#include "listing.h"
//#include "archive.h"

//#include "listing.h"

FILE *out_file;

char mem_block[512];

// int main(int argc, char **argv){
//     //if file already exists it needs to be wiped
//     //open file in w mode first, write nothing, close, reopen in a mode
    // out_file = fopen("testout.tar","w");
    // close(out_file);
    // out_file = fopen("testout.tar","a");
    // manage_file(argv[1]);
    // write_block(FORCE);
    // write_block(FORCE);


//     //handle case for t flag for list mode

//     //initializing file pointer to pass into 
//     FILE *in_file;

//     in_file = fopen(*argv[2], "rb");

//     //pass in 1 as 2nd arg if v flag is encountered
//     //check will be implemented here
//     print_archive(in_file, 1);


// }



int main(int argc, char **argv){

    //handle case for t flag for list mode

    //initializing file pointer to pass into 
    FILE *in_file;

    if ((in_file = fopen(argv[2], "rb")) == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[2]);
        return 1;
    }

    //pass in 1 as 2nd arg if v flag is encountered
    //check will be implemented here
    print_archive(in_file, 1);

    fclose(in_file);

}