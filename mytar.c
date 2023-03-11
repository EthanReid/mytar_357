#include "mytar.h"
FILE *out_file;

int main(int argc, char **argv){
    //if file already exists it needs to be wiped
    //open file in w mode first, write nothing, close, reopen in a mode
    out_file = fopen("testout.tar","w");
    close(out_file);
    out_file = fopen("testout.tar","a");
    manage_file(argv[1]);
    write_block(FORCE);
    write_block(FORCE);
}

char mem_block[512];


