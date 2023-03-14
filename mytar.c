#include "mytar.h"
//#include "listing.h"
//#include "archive.h"

//#include "listing.h"

FILE *out_file;

char mem_block[512];

/*temp func*/
void read_head(FILE *infile){
    fread(&head, sizeof(head),1, infile);
    printf("read");
}

int main(int argc, char **argv){
//     //if file already exists it needs to be wiped
//     //open file in w mode first, write nothing, close, reopen in a mode
/* 
    out_file = fopen("testout.tar","w");
    close(out_file);
    out_file = fopen("testout.tar","a");
    manage_file(argv[1]);
    write_block(FORCE);
    write_block(FORCE);
*/
    FILE *infile = fopen(argv[1], "rb");
    read_head(infile);

}

/*
int main(int argc, char **argv){
    //handle case for t flag for list mode
    argc_val = argc;
    in_file = fopen(argv[2], "rb");
    if (argc < 2) {
        perror("usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        return 1;
    }
    else{ 
        print_archive(in_file, 1, argv);
    }

    // if ((in_file = fopen(argv[2], "rb")) == NULL) {
    //     fprintf(stderr, "Error: Could not open file %s\n", argv[2]);
    //     return 1
    //     ;
    // }
    //pass in 1 as 2nd arg if v flag is encountered
    //check will be implemented here

    //if t flag invoked 


    

    fclose(in_file);

}

*/
